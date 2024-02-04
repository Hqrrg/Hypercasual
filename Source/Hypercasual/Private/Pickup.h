// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boulder.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UENUM()
enum EPickupRarity : uint8
{
	EPR_RARE = 0,
	EPR_UNCOMMON = 1,
	EPR_COMMON = 2
};

UCLASS()
class HYPERCASUAL_API APickup : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Appearance")
	UStaticMeshComponent* PickupMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	UMaterialInterface* PickupMaterial = nullptr;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* PickupBoxComponent = nullptr;

public:
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when actor is constructed
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frameW
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TEnumAsByte<EPickupRarity> PickupRarity = EPR_COMMON;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void PickedUp(ABoulder* Boulder);
};
