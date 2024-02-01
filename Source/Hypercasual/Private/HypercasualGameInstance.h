// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HypercasualSaveGame.h"
#include "Engine/GameInstance.h"
#include "HypercasualGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HYPERCASUAL_API UHypercasualGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHypercasualSaveGame* SaveGameInstance;
	
	void SaveGame();
	void LoadGame();

	int32 HighScore;
};
