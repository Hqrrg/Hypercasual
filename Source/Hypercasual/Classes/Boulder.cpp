// Fill out your copyright notice in the Description page of Project Settings.


#include "Boulder.h"
#include "Barrier.h"
#include "BoulderController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HypercasualGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoulder::ABoulder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoulderMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoulderMesh"));
	BoulderMeshComponent->SetSimulatePhysics(true);
	BoulderMeshComponent->BodyInstance.bNotifyRigidBodyCollision = true;
	SetRootComponent(BoulderMeshComponent);
}

void ABoulder::OnConstruction(const FTransform& Transform)
{
	if (BoulderMesh)
	{
		BoulderMeshComponent->SetStaticMesh(BoulderMesh);
		
		if (BoulderMaterial)
		{
			BoulderMesh->SetMaterial(0, BoulderMaterial);
		}
	}
}

// Called when the game starts or when spawned
void ABoulder::BeginPlay()
{
	Super::BeginPlay();

	SpawningLocationX = GetActorLocation().X;

	FTimerHandle ShiftWorldOriginHandle;
	GetWorldTimerManager().SetTimer(ShiftWorldOriginHandle, this, &ABoulder::ShiftWorldOrigin, 5.0f, true);
	
	GetWorldTimerManager().SetTimer(PhysicsMovementHandle, this, &ABoulder::ApplyPhysicsMovement, 0.01f, true);

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void ABoulder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DistanceTravelled = GetActorLocation().X - SpawningLocationX;
}

void ABoulder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		//Building
		EnhancedInputComponent->BindAction(BuildAction, ETriggerEvent::Triggered, this, &ABoulder::Build);
		EnhancedInputComponent->BindAction(BuildAction, ETriggerEvent::Completed, this, &ABoulder::CancelBuild);
	}
}

ABarrier* Barrier = nullptr;

void ABoulder::Build(const FInputActionValue &Value)
{
	if (!BuildTimerHandle.IsValid())
	{
		if (ABoulderController* BoulderController = Cast<ABoulderController>(Controller))
		{
			if (FHitResult* OutHit = BoulderController->GetWorldLocationFromMousePosition())
			{
				FActorSpawnParameters SpawnParams;
				FVector SpawnLoc = OutHit->Location;
				FRotator SpawnRot = FRotator(0.0f, 0.0f, 0.0f);

				Barrier = GetWorld()->SpawnActor<ABarrier>(SpawnLoc, SpawnRot, SpawnParams);

				GetWorldTimerManager().SetTimer(BuildTimerHandle, Barrier, &ABarrier::AddNextPoint, 0.001f, true, 0.0f);
			}
		}
	}
}

void ABoulder::CancelBuild(const FInputActionValue& Value)
{
	if (Barrier) Barrier = nullptr;
	if (BuildTimerHandle.IsValid()) GetWorldTimerManager().ClearTimer(BuildTimerHandle);
}

int32 ABoulder::GetRemainingLives()
{
	return RemainingLives;
}

void ABoulder::DecrementLives()
{
	if (!Ghosted)
	{
		RemainingLives--;

		if (AHypercasualGameMode* HypercasualGameMode = Cast<AHypercasualGameMode>(GetWorld()->GetAuthGameMode()))
		{
			if (RemainingLives > 0)
			{
				Ghost();
				return;
			}

			EndGame();
			HypercasualGameMode->EndGame();
		}
	}
}

void ABoulder::ApplyPhysicsMovement()
{
	const FVector ForceDirection = FVector(1.0f, 0.0f, 0.0f);
	const float Velocity = GetVelocity().Length();
	const float ForceScaleFactor = FMath::GetMappedRangeValueClamped(FVector2D(VelocityLimit-5, VelocityLimit), FVector2D(VelocityLimit, 0), Velocity);

	BoulderMeshComponent->AddForce(ForceDirection * ForceScaleFactor, NAME_None, true);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Velocity: %f"), Velocity));
}

bool GhostMaterialApplied;

void ABoulder::Ghost()
{
	Ghosted = !Ghosted;
	GhostMaterialApplied = false;
	
	if (Ghosted)
	{
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
		
		BoulderMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

		// Recursive Function Call After Timer Expiration
		GetWorldTimerManager().SetTimer(ResetGhostedTimerHandle, this, &ABoulder::Ghost, 3.0f);
		//GetWorldTimerManager().SetTimer(FlickerTimerHandle, this, &ABoulder::ToggleGhostMaterial, 0.5f, true, 0.0f);
		
	}
	else
	{
		//GetWorldTimerManager().ClearTimer(FlickerTimerHandle);
		GetWorldTimerManager().ClearTimer(ResetGhostedTimerHandle);
		
		BoulderMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
		//BoulderMeshComponent->SetMaterial(0, BoulderMaterial);
		
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
}

void ABoulder::ToggleGhostMaterial()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString("Hello"));
	if (BoulderGhostMaterial)
	{
		if (!GhostMaterialApplied)
		{
			BoulderMesh->SetMaterial(0, BoulderGhostMaterial);
		}
		else
		{
			BoulderMesh->SetMaterial(0, BoulderMaterial);
		}
		GhostMaterialApplied = !GhostMaterialApplied;

	}
}

void ABoulder::EndGame()
{
	GetWorldTimerManager().ClearTimer(PhysicsMovementHandle);
	BoulderMeshComponent->SetSimulatePhysics(false);
}

void ABoulder::ShiftWorldOrigin()
{
	const FVector ActorLocation = GetActorLocation();
	UWorld* World = GetWorld();

	if (FMath::Abs(ActorLocation.X) > 10000) 
	{
		World->SetNewWorldOrigin(FIntVector(ActorLocation.X, 0.0f, 0.0f) + World->OriginLocation);
		SpawningLocationX-=ActorLocation.X;
	}
}


