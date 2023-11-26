// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowCamera.h"
#include "Tile.h"
#include "HypercasualGameMode.h"


// Sets default values
AFollowCamera::AFollowCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);

	CullingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CullingBox"));
	CullingBox->SetBoxExtent(FVector(100.0f, 1000.0f, 1000.0f));
	CullingBox->SetRelativeLocation(FVector(-2000.0f, 0.0f, 0.0f));
	CullingBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFollowCamera::BeginPlay()
{
	Super::BeginPlay();

	CullingBox->OnComponentEndOverlap.AddDynamic(this, &AFollowCamera::CullingBoxEndOverlap);
}

// Called every frame
void AFollowCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FollowTarget)
	{
		FVector TargetLocation = FollowTarget->GetActorLocation();
		SetActorLocation(FVector(TargetLocation.X, 0.0f, TargetLocation.Z));
		
		FVector CameraTargetLocation = TargetLocation + Offset;
		CameraComponent->SetWorldLocation(FVector(CameraTargetLocation.X, 0.0f, CameraTargetLocation.Z));
	}
}

void AFollowCamera::SetPlayerController(APlayerController* NewPlayerController)
{
	PlayerController = NewPlayerController;
}

void AFollowCamera::SetFollowTarget(AActor* NewTarget)
{
	FollowTarget = NewTarget;
}

void AFollowCamera::SetOffset(float X, float Y, float Z)
{
	Offset = FVector(X, Y, Z);
}

void AFollowCamera::SetPitch(float Pitch)
{
	CameraComponent->SetRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
}

void AFollowCamera::CullingBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (ATile* OverlappedTile = Cast<ATile>(OtherActor))
	{
		if (UStaticMeshComponent* OtherStaticMeshComponent = Cast<UStaticMeshComponent>(OtherComponent))
		{
			if (OtherStaticMeshComponent == OverlappedTile->TileMeshComponent)
			{
				OverlappedTile->Destroy();

				if (AHypercasualGameMode* HypercasualGameMode = Cast<AHypercasualGameMode>(GetWorld()->GetAuthGameMode()))
				{
					HypercasualGameMode->SpawnNextTile();
				}
			}
		}
	}
}


