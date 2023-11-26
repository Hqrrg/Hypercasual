// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(SceneComponent);

	TileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMeshComponent"));
	TileMeshComponent->SetupAttachment(SceneComponent);
	TileMeshComponent->SetRelativeLocation(FVector(2500.0f, 0.0f, 0.0f));

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPointComponent"));
	AttachPoint->SetupAttachment(TileMeshComponent);
	AttachPoint->SetRelativeLocation(FVector(2500.0f, 0.0f, 0.0f));
	AttachPoint->SetArrowSize(5.0f);
}

void ATile::OnConstruction(const FTransform& FTransform)
{
	if (TileMesh) TileMeshComponent->SetStaticMesh(TileMesh);
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

