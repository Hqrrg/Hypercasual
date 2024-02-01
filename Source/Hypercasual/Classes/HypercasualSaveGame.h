// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HypercasualSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class HYPERCASUAL_API UHypercasualSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UHypercasualSaveGame();

	FString SaveSlotName;
	uint32 UserIndex;

	int32 HighScore;
};
