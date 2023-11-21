// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

//Defining custom trace channel
#define BARRIER_TRACE_CHANNEL ECollisionChannel::ECC_GameTraceChannel1

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(SceneComponent);

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMeshComponent"));
	TileMesh->SetupAttachment(SceneComponent);
	TileMesh->SetRelativeScale3D(FVector(50.0f, 50.0f, 50.0f));
	TileMesh->SetRelativeLocation(FVector(2500.0f, 0.0f, 0.0f));
	TileMesh->SetCollisionResponseToChannel(BARRIER_TRACE_CHANNEL, ECollisionResponse::ECR_Block);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TileMeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));

	if (TileMeshAsset.Succeeded()) 
	{
		TileMesh->SetStaticMesh(TileMeshAsset.Object);
	}

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPointComponent"));
	AttachPoint->SetupAttachment(TileMesh);
	AttachPoint->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
	AttachPoint->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FTransform ATile::GetAttachPointTransform() 
{
	FTransform AttachPointTransform = AttachPoint->GetComponentTransform();
	
	return FTransform(AttachPointTransform.GetRotation(), AttachPointTransform.GetLocation(), FVector(1.0f, 1.0f, 1.0f));
}

