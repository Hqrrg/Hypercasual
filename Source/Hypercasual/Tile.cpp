// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SceneComponent->SetupAttachment(RootComponent);

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMeshComponent"));
	TileMesh->SetupAttachment(SceneComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TileMeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));

	if (TileMeshAsset.Succeeded()) 
	{
		TileMesh->SetStaticMesh(TileMeshAsset.Object);
		TileMesh->SetRelativeScale3D(FVector(50, 50, 50));
		TileMesh->SetRelativeLocation(FVector(2500, 0, 0));
	}

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPointComponent"));
	AttachPoint->SetupAttachment(TileMesh);
	AttachPoint->SetRelativeLocation(FVector(50, 0 ,0));
	AttachPoint->SetUsingAbsoluteScale(true);
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

