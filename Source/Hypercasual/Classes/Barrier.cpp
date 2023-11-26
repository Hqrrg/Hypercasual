// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrier.h"
#include "Components/SplineMeshComponent.h"
#include "BoulderController.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABarrier::ABarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrierSpline = CreateDefaultSubobject<USplineComponent>(TEXT("BarrierSpline"));
	SetRootComponent(BarrierSpline);
	BarrierSpline->ClearSplinePoints();

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrierMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));

	if (BarrierMeshAsset.Succeeded()) BarrierMesh = BarrierMeshAsset.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BarrierMaterialAsset(TEXT("/Game/Hypercasual/Materials/M_Barrier.M_Barrier"));
	if (BarrierMaterialAsset.Succeeded()) BarrierMaterial = BarrierMaterialAsset.Object;
	
	if (BarrierMesh) BarrierMesh->SetMaterial(0, BarrierMaterial);
}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABarrier::AddNextPoint()
{
	ABoulderController* BoulderController = Cast<ABoulderController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (BarrierMesh && BoulderController)
	{
		FHitResult* OutHit = BoulderController->GetWorldLocationFromMousePosition();

		if (OutHit)
		{
			if (OutHit->PhysMaterial.Get())
			{
				UPhysicalMaterial* PhysicalMaterial = OutHit->PhysMaterial.Get();

				if (PhysicalMaterial->SurfaceType == EPhysicalSurface::SurfaceType1 && BarrierSpline->GetSplineLength() <= MAX_SPLINE_LENGTH)
				{
					FVector Loc = OutHit->Location;
					BarrierSpline->AddSplinePoint(FVector(Loc.X, Loc.Y, Loc.Z + (BarrierMesh->GetBoundingBox().GetSize().Y / 2)), ESplineCoordinateSpace::World, true);
					AddMeshComponents();

					if (!DecayTimerHandle.IsValid())
					{
						GetWorldTimerManager().SetTimer(DecayTimerHandle, this, &ABarrier::Decay, 0.5f, true, 0.5f);
					}
					return;
				}
			}
		}
	}

	// Force player to stop building spline
	FViewport* Viewport = GEngine->GameViewport->Viewport;

	FInputKeyEventArgs KeyEventArgs = FInputKeyEventArgs(
		Viewport,
		UGameplayStatics::GetPlayerControllerID(BoulderController),
		FKey(EKeys::LeftMouseButton.GetFName()),
		EInputEvent::IE_Released);

	Viewport->GetClient()->InputKey(KeyEventArgs);
}

void ABarrier::AddMeshComponents()
{
	// Change method for adding meshes to spline */
	for (int32 i = 0; i < BarrierSpline->GetNumberOfSplinePoints()-1; i++)
	{
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
		USplineMeshComponent* SplineMeshComponent = BarrierMeshComps[0];
		BarrierMeshComps.Remove(SplineMeshComponent);

		SplineMeshComponent->DestroyComponent();
		
		SplineMeshComponent = nullptr;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DecayTimerHandle);
		Destroy();
	}
}

