// Copyright Epic Games, Inc. All Rights Reserved.

#include "HypercasualGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Tile.h"

AHypercasualGameMode::AHypercasualGameMode()
{
	NextTileTransform = FTransform(FRotator(-15.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
}
 
void AHypercasualGameMode::BeginPlay()
{
	Super::BeginPlay();
	for (int32 i = 0; i < 3; i++)
	{
		SpawnNextTile();
	}
}

void AHypercasualGameMode::SpawnNextTile()
{
	if (LastTile) NextTileTransform = LastTile->GetAttachPointTransform();

	TArray<TSubclassOf<ATile>> TileArray;

	for (const TPair<TSubclassOf<ATile>, TileRarity> &Element : TileFabs)
	{
		for (int32 i = 0; i < Element.Value + 1; i++)
		{
			TileArray.Add(Element.Key);
		}
	}

	int32 Index = FMath::RandRange(0, TileArray.Num()-1);
	ATile* NextTile = GetWorld()->SpawnActor<ATile>(TileArray[Index]->GetAuthoritativeClass(), NextTileTransform);
	
	if (NextTile)
	{
		NextTile->FinishSpawning(NextTileTransform);
		LastTile = NextTile;
	}
}

void AHypercasualGameMode::EndGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString("Game Over!"));
}