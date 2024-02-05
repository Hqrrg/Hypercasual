// Fill out your copyright notice in the Description page of Project Settings.


#include "../Private/Pickups/LifePickup.h"
#include "Boulder.h"


// Sets default values
ALifePickup::ALifePickup()
{
	if (PickupInfoDataTable)
	{
		static const FString ContextString(TEXT("Pickup Info Context"));
		PickupInfo = PickupInfoDataTable->FindRow<FPickupInfo>(FName("Life"), ContextString, true);
	}
}

// Called when the game starts or when spawned
void ALifePickup::BeginPlay()
{
	Super::BeginPlay();
}

void ALifePickup::PickedUp(ABoulder* Boulder)
{
	Boulder->SetRemainingLives(Boulder->GetRemainingLives() + 1);
}

