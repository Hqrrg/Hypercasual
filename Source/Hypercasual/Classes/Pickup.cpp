// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Boulder.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupBoxComponent"));
	PickupBoxComponent->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
	PickupBoxComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(PickupBoxComponent);
	
	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMeshComponent"));
	PickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupMeshComponent->SetCastShadow(false);
	PickupMeshComponent->SetupAttachment(PickupBoxComponent);

	PickupNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PickupNiagaraComponent"));
	PickupNiagaraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -PickupBoxComponent->GetUnscaledBoxExtent().Z));
	PickupNiagaraComponent->SetupAttachment(PickupBoxComponent);

	static ConstructorHelpers::FObjectFinder<UDataTable> PickupInfoDataTableAsset(TEXT("/Game/Hypercasual/DataTables/DT_PickupInfo.DT_PickupInfo"));

	if (PickupInfoDataTableAsset.Succeeded())
	{
		PickupInfoDataTable = PickupInfoDataTableAsset.Object;
	}
}

void APickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	PickupBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APickup::BeginOverlap);

	if (PickupInfo)
	{
		UStaticMesh* PickupMesh = PickupInfo->StaticMesh;
		UMaterialInterface* PickupMaterial = PickupInfo->Material;
		UNiagaraSystem* NiagaraSystem = PickupInfo->NiagaraSystem;
	
		if (PickupMesh) PickupMeshComponent->SetStaticMesh(PickupMesh);
		if (PickupMaterial) PickupMeshComponent->SetMaterial(0, PickupMaterial);
		if (NiagaraSystem) PickupNiagaraComponent->SetAsset(NiagaraSystem);
	}
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

