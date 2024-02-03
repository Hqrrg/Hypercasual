// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HypercasualGameInstance.h"
#include "PickupComponent.h"
#include "Tile.h"
#include "GameFramework/GameModeBase.h"
#include "HypercasualGameMode.generated.h"

UENUM(BlueprintType)
enum ETileRarity : uint8
{
	ETR_Rare = 0,
	ETR_Uncommon = 1,
	ETR_Common = 2
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

	UPROPERTY()
	ATile* LastTile = nullptr;

public:
	UPROPERTY(BlueprintReadOnly)
	UHypercasualGameInstance* HypercasualGameInstance = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tiles", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATile> SpawningTile = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tiles", meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<ATile>, TEnumAsByte<ETileRarity>> TileFabs;
	
	UPROPERTY()
	bool HasBeatenRecord = false;
	
	ATile* SpawnNextTile();

	UFUNCTION(BlueprintNativeEvent)
	void EndGame(int32 DistanceTravelled);
};