// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PickupComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HYPERCASUAL_API UPickupComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPickupComponent();

protected:
	virtual void OnComponentCreated() override;
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void SpawnPickup();
};
