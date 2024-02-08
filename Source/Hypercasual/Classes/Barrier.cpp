// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrier.h"
#include "Components/SplineMeshComponent.h"
#include "BoulderController.h"
#include "Boulder.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABarrier::ABarrier()
{
	BarrierSpline = CreateDefaultSubobject<USplineComponent>(TEXT("BarrierSpline"));
	SetRootComponent(BarrierSpline);
	BarrierSpline->ClearSplinePoints();

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrierMeshAsset(TEXT("/Game/Hypercasual/Meshes/SM_Barrier.SM_Barrier"));

	if (BarrierMeshAsset.Succeeded()) BarrierMesh = BarrierMeshAsset.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BarrierMaterialAsset(TEXT("/Game/Hypercasual/Materials/Barrier/M_Barrier.M_Barrier"));
	if (BarrierMaterialAsset.Succeeded()) BarrierMaterial = BarrierMaterialAsset.Object;
	
	if (BarrierMesh && BarrierMaterial) BarrierMesh->SetMaterial(0, BarrierMaterial);

	LastPointPosition = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();
}

void ABarrier::SetUpgraded(bool IsUpgraded)
{
	Upgraded = IsUpgraded;

	if (IsUpgraded) MaxSplineLength = MaxSplineLength * 2;
}

void ABarrier::AddNextPoint()
{
	ABoulderController* BoulderController = Cast<ABoulderController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (BarrierMesh && BoulderController)
	{
		if (FHitResult* OutHit = BoulderController->GetHitFromMousePosition())
		{
			if (OutHit->PhysMaterial.Get())
			{
				UPhysicalMaterial* PhysicalMaterial = OutHit->PhysMaterial.Get();

				if (PhysicalMaterial->SurfaceType == EPhysicalSurface::SurfaceType1 && BarrierSpline->GetSplineLength() <= MaxSplineLength)
				{
					const FVector BarrierMeshBoundingBoxSize = BarrierMesh->GetBoundingBox().GetSize();
					
					if (const FVector Loc = OutHit->Location; (Loc-LastPointPosition).Length() >= BarrierMeshBoundingBoxSize.Length() && (Loc-BoulderController->GetPawn()->GetActorLocation()).Length() > BarrierMeshBoundingBoxSize.Length() + 10.0f)
					{
						FVector PointPosition = FVector(Loc.X, Loc.Y, Loc.Z + (BarrierMeshBoundingBoxSize.Z / 2));
						
						bool ShouldContinue = UpdateSplinePoints(LastPointPosition, PointPosition, BarrierMeshBoundingBoxSize.Length());
						
						if (!ShouldContinue) return;
						
						AddMeshComponents();
					}
					if (!Upgraded && !DecayTimerHandle.IsValid())
					{
						GetWorldTimerManager().SetTimer(DecayTimerHandle, this, &ABarrier::Decay, 0.15f, true, 0.15f);
					}
					return;
				}
			}
		}
	}

	// Force player to stop building spline 
	if (BoulderController)
	{
		if (ABoulder* Boulder = Cast<ABoulder>(BoulderController->GetPawn()))
		{
			FInputActionValue Value;
			Boulder->CancelBuild(Value);
		}
	}
}

bool ABarrier::UpdateSplinePoints(FVector PointA, FVector PointB, float DesiredDistance)
{
	if (IsUpgraded())
	{
		DesiredDistance = DesiredDistance * 2;
	}
	
	if (PointA == FVector::ZeroVector)
	{
		BarrierSpline->AddSplinePoint(PointB, ESplineCoordinateSpace::World, true);
		LastPointPosition = PointB;
		return false;
	}
	
	float Distance = (PointB - PointA).Length();
	int32 Div = FMath::Floor(Distance / DesiredDistance);
	
	if (Div > 1)
	{
		for (float i = 1; i < Div; i++)
		{
			FVector SplinePointLoc = FMath::Lerp(PointA, PointB, i/Div);
			BarrierSpline->AddSplinePoint(SplinePointLoc, ESplineCoordinateSpace::World, true);

			if (BarrierSpline->GetSplineLength() > MaxSplineLength)
			{
				BarrierSpline->RemoveSplinePoint(BarrierSpline->GetNumberOfSplinePoints()-1, true);
				break;
			}
			LastPointPosition = SplinePointLoc;
		}
	}
	else
	{
		BarrierSpline->AddSplinePoint(PointB, ESplineCoordinateSpace::World, true);

		if (BarrierSpline->GetSplineLength() > MaxSplineLength)
		{
			BarrierSpline->RemoveSplinePoint(BarrierSpline->GetNumberOfSplinePoints()-1, true);
			return false;
		}
		
		LastPointPosition = PointB;
		return true;
	}
	return true;
}

void ABarrier::AddMeshComponents()
{
	for (int32 i = 0; i < BarrierSpline->GetNumberOfSplinePoints()-1; i++)
	{
		BarrierSpline->GetSplinePointsPosition().Points[i].InterpMode = CIM_CurveAuto;
		
		USplineMeshComponent* BarrierMeshComponent = nullptr;

		if (BarrierMeshComps.Num() > i) BarrierMeshComponent = BarrierMeshComps[i];	
		
		if (!BarrierMeshComponent)
		{
			BarrierMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			
			if (BarrierMesh) BarrierMeshComponent->SetStaticMesh(BarrierMesh);

			BarrierMeshComponent->SetMobility(EComponentMobility::Movable);
			BarrierMeshComponent->AttachToComponent(BarrierSpline, FAttachmentTransformRules::KeepRelativeTransform);
			BarrierMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			BarrierMeshComponent->RegisterComponent();

			BarrierMeshComps.Add(BarrierMeshComponent);
		}
		
		FVector StartLoc = FVector(0.0f, 0.0f, 0.0f);
		FVector StartTan = FVector(0.0f, 0.0f, 0.0f);
		BarrierSpline->GetLocationAndTangentAtSplinePoint(i, StartLoc, StartTan, ESplineCoordinateSpace::Local);

		FVector EndLoc = FVector(0.0f, 0.0f, 0.0f);
		FVector EndTan = FVector(0.0f, 0.0f, 0.0f);
		BarrierSpline->GetLocationAndTangentAtSplinePoint(i+1, EndLoc, EndTan, ESplineCoordinateSpace::Local);

		BarrierMeshComponent->SetStartPosition(StartLoc);
		BarrierMeshComponent->SetStartTangent(StartTan);
		BarrierMeshComponent->SetEndPosition(EndLoc);
		BarrierMeshComponent->SetEndTangent(EndTan);
	}
}

void ABarrier::Decay()
{
	if (BarrierMeshComps.Num() != 0)
	{
		USplineMeshComponent* SplineMeshComponent = nullptr;
		
		for (int32 i = 0; i < BarrierMeshComps.Num(); i++)
		{
			if (USplineMeshComponent* CurrentBarrierMesh = BarrierMeshComps[i]; CurrentBarrierMesh->IsRegistered() && !CurrentBarrierMesh->IsBeingDestroyed())
			{
				SplineMeshComponent = CurrentBarrierMesh;
				break;
			}
		}

		if (SplineMeshComponent)
		{
			SplineMeshComponent->DestroyComponent();
		}
		else
		{
			GetWorldTimerManager().ClearTimer(DecayTimerHandle);
			Destroy();
		}
	}
}