// Fill out your copyright notice in the Description page of Project Settings.


#include "BoulderPawn.h"

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
	CameraBoom->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->SetFieldOfView(120.0f);
}

// Called when the game starts or when spawned
void ABoulderPawn::BeginPlay()
{
	Super::BeginPlay();
	
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

}

