// Fill out your copyright notice in the Description page of Project Settings.


#include "../Private/Pickups/BarrierPickup.h"

#include "Boulder.h"


// Sets default values
ABarrierPickup::ABarrierPickup()
{
	if (PickupInfoDataTable)
	{
		static const FString ContextString(TEXT("Pickup Info Context"));
		PickupInfo = PickupInfoDataTable->FindRow<FPickupInfo>(FName("Barrier"), ContextString, true);
	}
}

// Called when the game starts or when spawned
void ABarrierPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABarrierPickup::PickedUp(ABoulder* Boulder)
{
	Boulder->SetUpgradedBarrier(12.0f, true);
	
	if (PickupInfo)
	{
		Boulder->BoulderNiagaraActor->SpawnNiagaraSystem(PickupInfo->BoulderNiagaraSystem, 12.0f);
	}
}
