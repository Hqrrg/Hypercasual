// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "GameFramework/GameModeBase.h"
#include "HypercasualGameMode.generated.h"

UENUM(BlueprintType)
enum TileRarity : uint8
{
	RARE = 0,
	UNCOMMON = 1,
	COMMON = 2
};

UCLASS(minimalapi)
class AHypercasualGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHypercasualGameMode();
	
protected:
	virtual void BeginPlay() override;

	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, const FString& Options) override;

private:
	FTransform NextTileTransform;
	ATile* LastTile = nullptr;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tiles", meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<ATile>, TEnumAsByte<TileRarity>> TileFabs;

	void SpawnNextTile();
	void EndGame();
};