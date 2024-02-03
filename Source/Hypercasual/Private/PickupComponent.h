// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "PickupComponent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum EPickupRarity : uint8
{
	EPR_Rare = 0,
	EPR_Uncommon = 1,
	EPR_Common = 2
};

UCLASS(ClassGroup = ("Custom"), meta = (BlueprintSpawnableComponent))
class HYPERCASUAL_API UPickupComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	virtual void OnComponentCreated() override;
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
};
