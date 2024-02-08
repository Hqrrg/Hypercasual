// Fill out your copyright notice in the Description page of Project Settings.


#include "HypercasualGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UHypercasualGameInstance::Init()
{
	Super::Init();
	
	SaveGameInstance = Cast<UHypercasualSaveGame>(UGameplayStatics::CreateSaveGameObject(UHypercasualSaveGame::StaticClass()));
	ShouldDisplayMainMenu = true;
}

void UHypercasualGameInstance::SaveProfile()
{
	if (SaveGameInstance)
	{
		FAsyncSaveGameToSlotDelegate SavedDelegate;
		SavedDelegate.BindUObject(this, &UHypercasualGameInstance::SaveProfileDelegate);
		
		SaveGameInstance->HighScore = HighScore;
		
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex, SavedDelegate);
	}
}

void UHypercasualGameInstance::LoadProfile()
{
	if (SaveGameInstance)
	{
		FAsyncLoadGameFromSlotDelegate LoadedDelegate;
		LoadedDelegate.BindUObject(this, &UHypercasualGameInstance::LoadProfileDelegate);
	
		UGameplayStatics::AsyncLoadGameFromSlot(SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex, LoadedDelegate);
	}
}

void UHypercasualGameInstance::SaveProfileDelegate(const FString& SlotName, const int32 UserIndex, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Profile saved!"));
	}
}

void UHypercasualGameInstance::LoadProfileDelegate(const FString &SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
	if (UHypercasualSaveGame* LoadedGame = Cast<UHypercasualSaveGame>(LoadedGameData))
	{
		HighScore = LoadedGame->HighScore;
		UE_LOG(LogTemp, Warning, TEXT("Profile loaded!"));
	}

}
