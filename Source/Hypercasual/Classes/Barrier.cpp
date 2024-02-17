// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrier.h"
#include "Components/SplineMeshComponent.h"
#include "BoulderController.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"

#define BUILDING_PHYS_SURFACE EPhysicalSurface::SurfaceType1
// Sets default values
ABarrier::ABarrier()
{
	BarrierSpline = CreateDefaultSubobject<USplineComponent>(TEXT("BarrierSpline"));
	SetRootComponent(BarrierSpline);
	BarrierSpline->ClearSplinePoints();

	static ConstructorHelpers::FObjectFinder<UDataTable> BarrierDataTableAsset(TEXT("/Game/Hypercasual/DataTables/DT_BarrierInfo.DT_BarrierInfo"));

	if (BarrierDataTableAsset.Succeeded())
	{
		BarrierDataTable = BarrierDataTableAsset.Object;
	}

	if (BarrierDataTable)
	{
		static const FString ContextString(TEXT("Barrier Info Context"));
		BarrierInfo = BarrierDataTable->FindRow<FBarrierInfo>(FName("Default"), ContextString, true);
	}

	LastPointPosition = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();
}

void ABarrier::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (BarrierInfo)
	{
		BarrierMesh = BarrierInfo->Mesh;
		BarrierMaterial = BarrierInfo->Material;
	}
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
			if (UPhysicalMaterial* PhysicalMaterial = OutHit->PhysMaterial.Get())
			{
				if (PhysicalMaterial->SurfaceType == BUILDING_PHYS_SURFACE)
				{
					const FVector BarrierMeshBoundingBoxSize = BarrierMesh->GetBoundingBox().GetSize();
					
					if (const FVector Loc = OutHit->Location; (Loc-LastPointPosition).Length() >= BarrierMeshBoundingBoxSize.Length()
						&& (Loc-BoulderController->GetPawn()->GetActorLocation()).Length() > BarrierMeshBoundingBoxSize.Length() + 10.0f)
					{
						FVector PointPosition = FVector(Loc.X, Loc.Y, Loc.Z + (BarrierMeshBoundingBoxSize.Z / 2));
						
						bool ShouldContinue = UpdateSplinePoints(LastPointPosition, PointPosition, BarrierMeshBoundingBoxSize.Length());
						
						if (!ShouldContinue)
						{
							// Force release build input key
							UGameViewportClient* ViewportClient = GetGameInstance()->GetGameViewportClient();
							FViewport* Viewport = ViewportClient->Viewport;
								
							FInputKeyEventArgs Args = FInputKeyEventArgs(
									Viewport,
									0,
									FKey(EKeys::LeftMouseButton),
									IE_Released);

							ViewportClient->InputKey(Args);
							return;
						}
						
						AddMeshComponents();
					}
					if (!Upgraded && !GetWorld()->GetTimerManager().TimerExists(DecayTimerHandle))
					{
						GetWorldTimerManager().SetTimer(DecayTimerHandle, this, &ABarrier::Decay, 0.15f, true, 0.15f);
					}
				}
			}
		}
	}
}

// Calculate where to add new spline points
bool ABarrier::UpdateSplinePoints(FVector PointA, FVector PointB, float DesiredDistance)
{
	if (IsUpgraded())
	{
		// Allow more distance between spline points if the barrier is upgraded for lag compensation
		DesiredDistance = DesiredDistance * 2;
	}

	// If this is the first spline point, skip checks
	if (PointA == FVector::ZeroVector)
	{
		BarrierSpline->AddSplinePoint(PointB, ESplineCoordinateSpace::World, true);
		LastPointPosition = PointB;
		return true;
	}

	/*
	 * 1. Calculate how many times the desired distance goes into the distance between PointA & PointB (Div)
	 * 2. If it's more than once, lerp between PointA & PointB, with "i/Div" as the alpha.
	 * 
	 * i/Div: e.g. 1/2 equates to 0.5 or halfway between the two points.
	 * 
	 * 3. Add new spline points until loop completes or maximum spline length is reached.
	 */
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
				return i > 1;
			}
			LastPointPosition = SplinePointLoc;
		}
	}
	// Distance between PointA & B is acceptable, add spline point as normal
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
			if (BarrierMaterial) BarrierMeshComponent->SetMaterial(0, BarrierMaterial);

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
	if (BarrierMeshComps.Num() > 0)
	{
		USplineMeshComponent* SplineMeshComponent = nullptr;
		
		for (int32 i = 0; i < BarrierMeshComps.Num(); i++)
		{
			if (USplineMeshComponent* CurrentBarrierMesh = BarrierMeshComps[i])
			{
				if (CurrentBarrierMesh->IsRegistered() && !CurrentBarrierMesh->IsBeingDestroyed())
				{
					SplineMeshComponent = CurrentBarrierMesh;
					break;
				}
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