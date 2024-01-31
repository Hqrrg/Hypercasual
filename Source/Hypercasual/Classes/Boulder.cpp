// Fill out your copyright notice in the Description page of Project Settings.


#include "Boulder.h"
#include "BoulderController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HypercasualGameMode.h"
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

	BoulderMeshComponent->SetCollisionResponseToChannel(OBSTACLE_COLLISION_CHANNEL, ECR_Overlap);
}

void ABoulder::OnConstruction(const FTransform& Transform)
{
	if (BoulderMesh)
	{
		BoulderMeshComponent->SetStaticMesh(BoulderMesh);
	}
}

// Called when the game starts or when spawned
void ABoulder::BeginPlay()
{
	Super::BeginPlay();

	if (BoulderMaterial)
	{
		DynamicBoulderMaterial = UMaterialInstanceDynamic::Create(BoulderMaterial, this);
		BoulderMeshComponent->SetMaterial(0, DynamicBoulderMaterial);
	}

	SpawningLocationX = GetActorLocation().X;

	FTimerHandle ShiftWorldOriginTimerHandle;
	GetWorldTimerManager().SetTimer(ShiftWorldOriginTimerHandle, this, &ABoulder::ShiftWorldOrigin, 5.0f, true);
	
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
	DistanceTravelled = (GetActorLocation().X - SpawningLocationX) / 10;
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

void ABoulder::Build(const FInputActionValue &Value)
{
	if (!BuildTimerHandle.IsValid())
	{
		if (ABoulderController* BoulderController = Cast<ABoulderController>(Controller))
		{
			if (FHitResult* OutHit = BoulderController->GetHitFromMousePosition())
			{
				FActorSpawnParameters SpawnParams;
				FVector SpawnLoc = OutHit->Location;
				FRotator SpawnRot = FRotator(0.0f, 0.0f, 0.0f);

				CurrentBarrier = GetWorld()->SpawnActor<ABarrier>(SpawnLoc, SpawnRot, SpawnParams);

				GetWorldTimerManager().SetTimer(BuildTimerHandle, CurrentBarrier, &ABarrier::AddNextPoint, 0.001f, true, 0.0f);
			}
		}
	}
}

void ABoulder::CancelBuild(const FInputActionValue& Value)
{
	if (CurrentBarrier) CurrentBarrier = nullptr;
	if (BuildTimerHandle.IsValid()) GetWorldTimerManager().ClearTimer(BuildTimerHandle);
}

bool ABoulder::Kill()
{
	if (!Immune)
	{
		RemainingLives--;

		if (AHypercasualGameMode* HypercasualGameMode = Cast<AHypercasualGameMode>(GetWorld()->GetAuthGameMode()))
		{
			if (RemainingLives > 0)
			{
				ToggleImmunity();
				return false;
			}

			StopPhysicsMovement();
			HypercasualGameMode->EndGame();
			return true;
		}
	}
	return false;
}

void ABoulder::Move()
{
	const FVector Velocity = GetVelocity();
	const FVector ForceDirection = FVector(1.0f, 0.0f, 0.0f);
	const float ForceMultiplier = FMath::GetMappedRangeValueClamped(FVector2D(VelocityLimit-5, VelocityLimit), FVector2D(VelocityLimit, 0), Velocity.Length());

	BoulderMeshComponent->AddForce(ForceDirection * ForceMultiplier, NAME_None, true);

	const FRotator ActorRotation = GetActorRotation();
	const FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + Velocity);
	//SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), FRotator(ActorRotation.Pitch, DesiredRotation.Yaw, ActorRotation.Roll), UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0.1f));
}

void ABoulder::ToggleImmunity()
{
	Immune = !Immune;
	
	if (Immune)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
		
		BoulderMeshComponent->SetCollisionResponseToChannel(OBSTACLE_COLLISION_CHANNEL, ECR_Ignore);

		// Recursive Function Call After Timer Expiration
		GetWorldTimerManager().SetTimer(ResetImmunityTimerHandle, this, &ABoulder::ToggleImmunity, 1.5f);
		GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &ABoulder::ImmunityBlink, 0.05f, true, 0.0f);
		
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ResetImmunityTimerHandle);
		GetWorldTimerManager().ClearTimer(BlinkTimerHandle);

		BoulderMeshComponent->SetCollisionResponseToChannel(OBSTACLE_COLLISION_CHANNEL, ECR_Overlap);
		if (DynamicBoulderMaterial) DynamicBoulderMaterial->SetVectorParameterValue("Colour", FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
}

void ABoulder::ImmunityBlink()
{
	FLinearColor Colour;

	switch (Blinked)
	{
	case true:
		Colour = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
		break;
		
	case false:
		Colour = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
		break;
	}

	if (DynamicBoulderMaterial) DynamicBoulderMaterial->SetVectorParameterValue("Colour", Colour);
	
	Blinked = !Blinked;
}

void ABoulder::StopPhysicsMovement()
{
	GetWorldTimerManager().ClearTimer(MovementTimerHandle);
	BoulderMeshComponent->SetSimulatePhysics(false);
}

void ABoulder::ShiftWorldOrigin()
{
	const FVector ActorLocation = GetActorLocation();
	UWorld* World = GetWorld();

	if (FMath::Abs(ActorLocation.X) > 50000) 
	{
		World->SetNewWorldOrigin(FIntVector(ActorLocation.X, 0.0f, 0.0f) + World->OriginLocation);
		SpawningLocationX-=ActorLocation.X;

		// Avoid being sent into the stratosphere
		SetActorLocation(FVector(0.0f, ActorLocation.Y, ActorLocation.Z));
	}
}


