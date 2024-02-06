// Fill out your copyright notice in the Description page of Project Settings.


#include "../Private/Pickups/ImmunityPickup.h"

#include "Boulder.h"


// Sets default values
AImmunityPickup::AImmunityPickup()
{
	if (PickupInfoDataTable)
	{
		static const FString ContextString(TEXT("Pickup Info Context"));
		PickupInfo = PickupInfoDataTable->FindRow<FPickupInfo>(FName("Immunity"), ContextString, true);
	}
}

// Called when the game starts or when spawned
void AImmunityPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AImmunityPickup::PickedUp(ABoulder* Boulder)
{
	Boulder->ToggleTemporaryVelocityBoost(3.0f, true);
	Boulder->ToggleImmunity(false, true, 6.0f);

	if (PickupInfo)
	{
		Boulder->BoulderNiagaraActor->SpawnNiagaraSystem(PickupInfo->BoulderNiagaraSystem, 6.0f);
	}
}
