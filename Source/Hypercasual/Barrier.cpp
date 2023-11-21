// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrier.h"
#include "Components/SplineMeshComponent.h"
#include "BoulderController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABarrier::ABarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrierSpline = CreateDefaultSubobject<USplineComponent>(TEXT("BarrierSpline"));
	SetRootComponent(BarrierSpline);
	BarrierSpline->ClearSplinePoints();

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrierMeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));

	if (BarrierMeshAsset.Succeeded()) BarrierMesh = BarrierMeshAsset.Object;
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
	if (BoulderController)
	{
		if (BarrierMesh)
		{
			FVector Loc = BoulderController->GetWorldLocationFromMousePosition();
			
			if (Loc != FVector(0.0f, 0.0f, 0.0f))
			{
				BarrierSpline->AddSplinePoint(FVector(Loc.X, Loc.Y, Loc.Z + (BarrierMesh->GetBoundingBox().GetSize().Y / 2)), ESplineCoordinateSpace::World, true);
				AddMeshComponents();
			}

		}
	}
}

void ABarrier::AddMeshComponents()
{
	for (int32 i = 0; i < BarrierSpline->GetNumberOfSplinePoints(); i++)
	{
		USplineMeshComponent* BarrierMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		if (BarrierMeshComponent)
		{
			if (BarrierMesh)
			{
				BarrierMeshComponent->SetStaticMesh(BarrierMesh);
			}

			BarrierMeshComponent->SetMobility(EComponentMobility::Movable);
			BarrierMeshComponent->AttachToComponent(BarrierSpline, FAttachmentTransformRules::KeepRelativeTransform);
			BarrierMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

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

			BarrierMeshComponent->RegisterComponent();
		}
	}
}

