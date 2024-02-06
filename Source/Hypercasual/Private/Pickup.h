// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UENUM()
enum EPickupRarity : uint8
{
	EPR_VeryRare = 0,
	EPR_Rare = 1,
	EPR_Uncommon = 2,
	EPR_Common = 3
};

USTRUCT(BlueprintType)
struct FPickupInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMesh* StaticMesh = nullptr;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material = nullptr;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraSystem = nullptr;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* BoulderNiagaraSystem = nullptr;
};

UCLASS()
class HYPERCASUAL_API APickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup();

	UPROPERTY(VisibleAnywhere, Category = "Appearance")
	UStaticMeshComponent* PickupMeshComponent = nullptr;

	UPROPERTY(EditAnywhere = "Appearance")
	UNiagaraComponent* PickupNiagaraComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* PickupBoxComponent = nullptr;

protected:
	// Called when actor is constructed
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frameW
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void PickedUp(class ABoulder* Boulder);

public:
	FPickupInfo* PickupInfo = nullptr;
	inline static UDataTable* PickupInfoDataTable = nullptr;
};
