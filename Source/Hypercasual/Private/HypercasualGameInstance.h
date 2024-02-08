// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HypercasualSaveGame.h"
#include "HypercasualGameInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNewHighScoreDelegate);

UCLASS()
class HYPERCASUAL_API UHypercasualGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
	
public:
	UPROPERTY()
	UHypercasualSaveGame* SaveGameInstance = nullptr;
	
	int32 HighScore = 0;

	void SaveProfile();
	void LoadProfile();

	UFUNCTION()
	void SaveProfileDelegate(const FString &SlotName, const int32 UserIndex, bool Success);
	
	UFUNCTION()
	void LoadProfileDelegate(const FString &SlotName, const int32 UserIndex, USaveGame* LoadedGameData);

	UPROPERTY(BlueprintAssignable)
	FNewHighScoreDelegate OnNewHighScore;
};
