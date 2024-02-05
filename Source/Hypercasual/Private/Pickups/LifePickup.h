// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "LifePickup.generated.h"

UCLASS()
class HYPERCASUAL_API ALifePickup : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALifePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	virtual void PickedUp(ABoulder* Boulder) override;
};
