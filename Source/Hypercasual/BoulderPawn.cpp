// Fill out your copyright notice in the Description page of Project Settings.


#include "BoulderPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BoulderController.h"
#include "Kismet/GameplayStatics.h"
#include "Barrier.h"

// Sets default values
ABoulderPawn::ABoulderPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(SceneComponent);

	BoulderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoulderMesh"));
	BoulderMesh->SetupAttachment(SceneComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoulderMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	if (BoulderMeshAsset.Succeeded())
	{
		BoulderMesh->SetStaticMesh(BoulderMeshAsset.Object);
		BoulderMesh->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
		BoulderMesh->SetSimulatePhysics(true);
	}


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(SceneComponent);
	CameraBoom->TargetArmLength = 1200.0f;
	CameraBoom->SetRelativeRotation(FRotator(-75.0f, 0.0f, 0.0f));
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->SetFieldOfView(120.0f);
}

// Called when the game starts or when spawned
void ABoulderPawn::BeginPlay()
{
	Super::BeginPlay();

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
void ABoulderPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(FVector(BoulderMesh->GetComponentLocation().X + 400.0f, GetActorLocation().Y, BoulderMesh->GetComponentLocation().Z));

	/* Error: Causing lighting artifacts.

	//Shift world origin to avoid floating point errors.
	FVector BoulderLoc = BoulderMesh->GetComponentLocation();
	UWorld* World = GetWorld();

	if (FMath::Abs(BoulderLoc.X) > 10000 || FMath::Abs(BoulderLoc.Y) > 10000 || FMath::Abs(BoulderLoc.Z) > 10000) 
	{
		World->SetNewWorldOrigin(FIntVector(BoulderLoc.X, BoulderLoc.Y, BoulderLoc.Z) + World->OriginLocation);
		//Fix for shifting world origin with a physics simulated object.
		BoulderMesh->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	}
	*/
}

// Called to bind functionality to input
void ABoulderPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
	//Building
	EnhancedInputComponent->BindAction(BuildAction, ETriggerEvent::Triggered, this, &ABoulderPawn::Build);
	EnhancedInputComponent->BindAction(BuildAction, ETriggerEvent::Completed, this, &ABoulderPawn::Build);
	}
}

ABarrier* Barrier = nullptr;

void ABoulderPawn::Build(const FInputActionValue &Value)
{
	if (Value.Get<bool>())
	{
		if (!BuildTimerHandle.IsValid())
		{
			ABoulderController* BoulderController = Cast<ABoulderController>(Controller);
			if (BoulderController)
			{
				FActorSpawnParameters SpawnParams;
				FVector SpawnLoc = BoulderController->GetWorldLocationFromMousePosition();
				FRotator SpawnRot = FRotator(0.0f, 0.0f, 0.0f);

				Barrier = GetWorld()->SpawnActor<ABarrier>(SpawnLoc, SpawnRot, SpawnParams);

				GetWorldTimerManager().SetTimer(BuildTimerHandle, Barrier, &ABarrier::AddNextPoint, 0.1f, true, 0.0f);
			}
		}
	}
	else
	{
		if (Barrier)
		{
			Barrier->SetLifeSpan(5.0f);
		}
		GetWorldTimerManager().ClearTimer(BuildTimerHandle);
	}
}

