// Fill out your copyright notice in the Description page of Project Settings.


#include "Boulder.h"
#include "Barrier.h"
#include "BoulderController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ABoulder::ABoulder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoulderMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoulderMesh"));
	BoulderMeshComponent->SetSimulatePhysics(true);
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

	FTimerHandle PhysicsMovementHandle;
	GetWorldTimerManager().SetTimer(PhysicsMovementHandle, this, &ABoulder::ApplyPhysicsMovement, 0.001f, true);

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

				GetWorldTimerManager().SetTimer(BuildTimerHandle, Barrier, &ABarrier::AddNextPoint, 0.1f, true, 0.0f);
			}
		}
	}
}

void ABoulder::CancelBuild(const FInputActionValue& Value)
{
	if (Barrier) Barrier = nullptr;
	if (BuildTimerHandle.IsValid()) GetWorldTimerManager().ClearTimer(BuildTimerHandle);
}

int32 ABoulder::SetMaxLinearVelocity(int32 Velocity)
{
	return MaxLinearVelocity = Velocity;
}

void ABoulder::ApplyPhysicsMovement()
{
	BoulderMeshComponent->AddForce(FVector(50.0f, 0.0f, 1.0f), FName(), true);
	
	FVector PhysicsLinearVelocity = BoulderMeshComponent->GetPhysicsLinearVelocity();

	if (PhysicsLinearVelocity.Length() > MaxLinearVelocity)
	{
		BoulderMeshComponent->SetPhysicsLinearVelocity(PhysicsLinearVelocity * (MaxLinearVelocity / PhysicsLinearVelocity.Length()), false);
	}
}

void ABoulder::ShiftWorldOrigin()
{
	FVector ActorLocation = GetActorLocation();
	UWorld* World = GetWorld();

	if (FMath::Abs(ActorLocation.X) > 10000 || FMath::Abs(ActorLocation.Y) > 10000 || FMath::Abs(ActorLocation.Z) > 10000) 
	{
		World->SetNewWorldOrigin(FIntVector(ActorLocation.X, 0.0f, ActorLocation.Z) + World->OriginLocation);
		SpawningLocationX-=ActorLocation.X;
	}
}


