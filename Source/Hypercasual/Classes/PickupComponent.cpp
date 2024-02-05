// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"
#include "Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "HypercasualGameMode.h"

// Sets default values for this component's properties
UPickupComponent::UPickupComponent()
{
	bHiddenInGame = true;
}

void UPickupComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
	ShapeColor = FColor(0, 0, 255);
	SetGenerateOverlapEvents(false);
}

// Called when the game starts
void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnPickup();
}

void UPickupComponent::SpawnPickup()
{
	int32 DiceResult = FMath::RandRange(1, 12);
	
	if (DiceResult == 12)
	{
		if (AHypercasualGameMode* HypercasualGameMode = Cast<AHypercasualGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			TArray<TSubclassOf<APickup>> PickupArray;

			for (const TPair<TSubclassOf<APickup>, EPickupRarity> Element : HypercasualGameMode->Pickups)
			{
				for (int i = 0; i < Element.Value + 1; i++)
				{
					PickupArray.Add(Element.Key);
				}
			}

			FTransform SpawnTransform = GetComponentTransform();
			int Index = FMath::RandRange(0, PickupArray.Num()-1);
		
			APickup* Pickup = GetWorld()->SpawnActor<APickup>(PickupArray[Index]->GetAuthoritativeClass(), SpawnTransform);
		
			if (Pickup)
			{
				Pickup->FinishSpawning(SpawnTransform);
			}
		}
	}
	DestroyComponent();
}

