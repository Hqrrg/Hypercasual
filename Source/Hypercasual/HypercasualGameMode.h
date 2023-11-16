// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

private:
	void SpawnNextTile();

	FTransform NextTileTransform;
};




