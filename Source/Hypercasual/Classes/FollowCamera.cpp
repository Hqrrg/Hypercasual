// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowCamera.h"
#include "Tile.h"
#include "Barrier.h"
#include "HypercasualGameMode.h"

#define OBSTACLE_COLLISION_CHANNEL ECC_EngineTraceChannel1

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
	CullingBox->SetupAttachment(RootComponent);
	CullingBox->SetBoxExtent(FVector(10.0f, 500.0f, 500.0f));
	CullingBox->SetRelativeLocation(FVector(-750.0f, 0.0f, 0.0f));
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
		FVector NewActorLocation = FVector(TargetLocation.X, FMath::Clamp(TargetLocation.Y, -150.0 , 150.0), TargetLocation.Z);
		
		SetActorLocation(NewActorLocation);
		CameraComponent->SetWorldLocation(NewActorLocation + Offset);

		EnsureFollowTargetIsVisible();
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
	else if (APickup* OverlappedPickup = Cast<APickup>(OtherActor))
	{
		OverlappedPickup->Destroy();
	}
	else if (ABarrier* OverlappedBarrier = Cast<ABarrier>(OtherActor))
	{
		OverlappedBarrier->Destroy();
	}
}

void AFollowCamera::EnsureFollowTargetIsVisible()
{
	if (FollowTarget)
	{
		FVector TraceStartLoc = CameraComponent->GetComponentLocation();
		FVector TraceEndLoc = FollowTarget->GetActorLocation();

		FHitResult* OutHit = new FHitResult();
		FCollisionQueryParams QueryParams;
		FCollisionObjectQueryParams ObjectQueryParams;
		
		ObjectQueryParams.AddObjectTypesToQuery(OBSTACLE_COLLISION_CHANNEL);

		bool LineTrace = GetWorld()->LineTraceSingleByObjectType(
			*OutHit,
			TraceStartLoc,
			TraceEndLoc,
			ObjectQueryParams,
			QueryParams);
		
		if (LineTrace)
		{
			if (UObstacleStaticMeshComponent* HitObstacleComponent = Cast<UObstacleStaticMeshComponent>(OutHit->GetComponent()))
			{
				if (VisibilityBlockingObstacle)
				{
					if (VisibilityBlockingObstacle != HitObstacleComponent)
					{
						VisibilityBlockingObstacle->SetPassthrough(false);
					}
				}
				VisibilityBlockingObstacle = HitObstacleComponent;
				if (!VisibilityBlockingObstacle->IsPassthrough) VisibilityBlockingObstacle->SetPassthrough(true);
			}
		}
		else
		{
			if (VisibilityBlockingObstacle)
			{
				VisibilityBlockingObstacle->SetPassthrough(false);
				VisibilityBlockingObstacle = nullptr;
			}
		}
	}
}


