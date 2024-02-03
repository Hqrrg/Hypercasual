// Fill out your copyright notice in the Description page of Project Settings.


#include "Boulder.h"
#include "BoulderController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#define OBSTACLE_COLLISION_CHANNEL ECC_EngineTraceChannel1

// Sets default values
ABoulder::ABoulder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoulderMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoulderMesh"));
	BoulderMeshComponent->SetSimulatePhysics(true);
	SetRootComponent(BoulderMeshComponent);

	// Set collision response for obstacle's to overlap
	BoulderMeshComponent->SetCollisionResponseToChannel(OBSTACLE_COLLISION_CHANNEL, ECR_Overlap);
}

void ABoulder::OnConstruction(const FTransform& Transform)
{
	if (BoulderMesh)
	{
		BoulderMeshComponent->SetStaticMesh(BoulderMesh);

		if (BoulderMaterial)
		{
			BoulderMeshComponent->SetMaterial(0, BoulderMaterial);
		}
	}
}

// Called when the game starts or when spawned
void ABoulder::BeginPlay()
{
	Super::BeginPlay();

	HypercasualGameMode = Cast<AHypercasualGameMode>(GetWorld()->GetAuthGameMode());
	
	SpawningLocationX = GetActorLocation().X;

	// Initialise timer for checking and shifting world origin
	FTimerHandle ShiftWorldOriginTimerHandle;
	GetWorldTimerManager().SetTimer(ShiftWorldOriginTimerHandle, this, &ABoulder::ShiftWorldOrigin, 5.0f, true);

	// Initialise timer for physics movement
	GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &ABoulder::Move, 0.01f, true);

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

	// Update distance travelled, dividing by 10 for a nicer number
	DistanceTravelled = (GetActorLocation().X - SpawningLocationX) / 10;

	const int32 CurrentHighScore = HypercasualGameMode->HypercasualGameInstance->Record;
	
	// If distance travelled is a new high score, notify listeners
	if (!HypercasualGameMode->HasBeatenRecord && DistanceTravelled > CurrentHighScore && CurrentHighScore > 0)
	{
		HypercasualGameMode->HypercasualGameInstance->OnNewRecord.Broadcast();
		HypercasualGameMode->HasBeatenRecord = true;
	}

	// If the current acceleration has not surpassed the velocity limit, increment it every specified interval of distance travelled
	if (DistanceTravelled != LastVelocityIncreaseInterval && DistanceTravelled % VelocityIncreaseInterval == 0 && Acceleration < VelocityLimit)
	{
		LastVelocityIncreaseInterval = DistanceTravelled;
		Acceleration+=50.0f;
		Acceleration = UKismetMathLibrary::Clamp(Acceleration, 0, VelocityLimit);
	}
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

// Build spline when input key is held down
void ABoulder::Build(const FInputActionValue &Value)
{
	// Check not already building
	if (!BuildTimerHandle.IsValid())
	{
		if (ABoulderController* BoulderController = Cast<ABoulderController>(Controller))
		{
			// Get hit position from mouse cursor in world
			if (FHitResult* OutHit = BoulderController->GetHitFromMousePosition())
			{
				FActorSpawnParameters SpawnParams;
				FVector SpawnLoc = OutHit->Location;
				FRotator SpawnRot = FRotator(0.0f, 0.0f, 0.0f);

				// Spawn barrier at hit position
				CurrentBarrier = GetWorld()->SpawnActor<ABarrier>(SpawnLoc, SpawnRot, SpawnParams);

				// Initialise timer to recursively call the barrier's AddNextPoint function
				GetWorldTimerManager().SetTimer(BuildTimerHandle, CurrentBarrier, &ABarrier::AddNextPoint, 0.01f, true, 0.0f);
			}
		}
	}
}

// Nullify current barrier and clear build timer when input key is released
void ABoulder::CancelBuild(const FInputActionValue& Value)
{
	if (CurrentBarrier) CurrentBarrier = nullptr;
	if (BuildTimerHandle.IsValid()) GetWorldTimerManager().ClearTimer(BuildTimerHandle);
}

// Attempts to kill the player/end the session
bool ABoulder::Kill()
{
	// Check the player is not currently immune
	if (!Immune)
	{
		// Decrement lives by 1
		RemainingLives--;

		if (HypercasualGameMode)
		{
			// Give temporary immunity to the player if they have remaining lives
			if (RemainingLives > 0)
			{
				ToggleImmunity();
				return false;
			}

			// Killed, end session
			StopPhysicsMovement();
			HypercasualGameMode->EndGame(DistanceTravelled);
			return true;
		}
	}
	return false;
}

// Applies controlled physics force on the boulder
void ABoulder::Move()
{
	const FVector Velocity = GetVelocity();
	// Direction to apply force in, default forward
	FVector ForceDirection = FVector(1.0f, 0.0f, 0.0f);

	// Moving
	if (Velocity.Length() > 0)
	{
		// Change force direction to match that of velocity
		ForceDirection = FVector(UKismetMathLibrary::Abs(Velocity.X), Velocity.Y, Velocity.Z);
	}
	// Determine force to be applied based on the boulder's current velocity
	const float ForceMultiplier = FMath::GetMappedRangeValueClamped(FVector2D(Acceleration-5, Acceleration), FVector2D(Acceleration, 0), Velocity.Length());

	// Normalise direction vector
	if (ForceDirection.Normalize())
	{
		// Apply force
		BoulderMeshComponent->AddForce(ForceDirection * ForceMultiplier, NAME_None, true);
	}
}

// Toggles the player's immune state
void ABoulder::ToggleImmunity()
{
	Immune = !Immune;
	
	if (Immune)
	{
		// Decrease game time to half - DISABLED | Sometimes does not trigger when toggling immunity in quick succession
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);

		Acceleration = DefaultAcceleration;
		
		// Set collision response for obstacle's to ignore
		BoulderMeshComponent->SetCollisionResponseToChannel(OBSTACLE_COLLISION_CHANNEL, ECR_Ignore);

		// Recursive function call after timer expiration, 1.0f = 2s due to time dilation
		GetWorldTimerManager().SetTimer(ResetImmunityTimerHandle, this, &ABoulder::ToggleImmunity, 3.0f);
		// Initialise looping timer to rapidly change the actor's visibility in game
		GetWorldTimerManager().SetTimer(ToggleVisibilityTimerHandle, this, &ABoulder::ToggleVisibility, 0.1f, true, 0.0f);
		
	}
	else
	{
		// Once no longer immune, clear previously instantiated timers
		GetWorldTimerManager().ClearTimer(ResetImmunityTimerHandle);
		GetWorldTimerManager().ClearTimer(ToggleVisibilityTimerHandle);

		// Set collision response for obstacle's to overlap
		BoulderMeshComponent->SetCollisionResponseToChannel(OBSTACLE_COLLISION_CHANNEL, ECR_Overlap);
		// Ensure actor is visible
		SetActorHiddenInGame(false);

		// Reset game time to normal - DISABLED | Sometimes does not trigger when toggling immunity in quick succession
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
}

// Toggles actor visibility in game
void ABoulder::ToggleVisibility()
{
	SetActorHiddenInGame(!IsHidden());
}

// Clears movement timer and stops simulating physics
void ABoulder::StopPhysicsMovement()
{
	GetWorldTimerManager().ClearTimer(MovementTimerHandle);
	BoulderMeshComponent->SetSimulatePhysics(false);
}

// Shifts the world origin to prevent floating point precision errors
void ABoulder::ShiftWorldOrigin()
{
	const FVector ActorLocation = GetActorLocation();
	UWorld* World = GetWorld();

	// Checks if the actor is far enough from world origin
	if (FMath::Abs(ActorLocation.X) > 50000) 
	{
		// Shift the world origin to the actor's location on the X axis
		World->SetNewWorldOrigin(FIntVector(ActorLocation.X, 0.0f, 0.0f) + World->OriginLocation);
		// Update spawning location to maintain correct value for distance travelled
		SpawningLocationX-=ActorLocation.X;

		// Avoid being sent into the stratosphere
		SetActorLocation(FVector(0.0f, ActorLocation.Y, ActorLocation.Z));
	}
}


