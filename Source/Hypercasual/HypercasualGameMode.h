// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HypercasualGameMode.generated.h"

UCLASS(minimalapi)
class AHypercasualGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	void SpawnNextTile();

	FTransform NextTileTransform;

protected:
	virtual void BeginPlay() override;

public:
	AHypercasualGameMode();
};




