// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HypercasualGameInstance.h"
#include "Pickup.h"
#include "Tile.h"
#include "GameFramework/GameModeBase.h"
#include "HypercasualGameMode.generated.h"

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

	UPROPERTY()
	ATile* LastTile = nullptr;

public:
	UPROPERTY(BlueprintReadOnly)
	UHypercasualGameInstance* HypercasualGameInstance = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tiles", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATile> SpawningTile = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tiles", meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<ATile>, TEnumAsByte<ETileRarity>> TileFabs;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups", meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<APickup>, TEnumAsByte<EPickupRarity>> Pickups;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MainMenuClass = nullptr;
	
	UPROPERTY()
	bool HasNewHighScore = false;
	
	ATile* SpawnNextTile();

	UFUNCTION(BlueprintNativeEvent)
	void EndGame(float DistanceTravelled, bool NewHighScore);
};