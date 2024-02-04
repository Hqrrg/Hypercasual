// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupBoxComponent"));
	PickupBoxComponent->SetBoxExtent(FVector(8.0f, 32.0f, 32.0f));
	SetRootComponent(PickupBoxComponent);
	
	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMeshComponent"));
	PickupMeshComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	PickupMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PickupMeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));
	
	if (PickupMeshAsset.Succeeded())
	{
		PickupMeshComponent->SetStaticMesh(PickupMeshAsset.Object);
	}
	PickupMeshComponent->SetupAttachment(PickupBoxComponent);
}

void APickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	PickupBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APickup::BeginOverlap);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABoulder* Boulder = Cast<ABoulder>(OtherActor))
	{
		PickedUp(Boulder);
		Destroy();
	}
}

void APickup::PickedUp(ABoulder* Boulder)
{
}

