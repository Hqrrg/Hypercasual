// Copyright Epic Games, Inc. All Rights Reserved.

#include "HypercasualGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Tile.h"
#include "BoulderController.h"
#include "Boulder.h"
#include "FollowCamera.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AHypercasualGameMode::AHypercasualGameMode()
{
	NextTileTransform  = FTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
}
 
void AHypercasualGameMode::BeginPlay()
{
	Super::BeginPlay();

	HypercasualGameInstance = Cast<UHypercasualGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	HypercasualGameInstance->LoadProfile();

	if (SpawningTile)
	{
		// Spawn default tile at 0,0,0
		ATile* SpawningTileInstance = GetWorld()->SpawnActor<ATile>(SpawningTile->GetAuthoritativeClass(), NextTileTransform);
		NextTileTransform = SpawningTileInstance->GetAttachPointTransform();
	}

	// Spawn 5 extra tiles at the start of the level
	for (int32 i = 0; i < 5; i++)
	{
		SpawnNextTile();
	}

	if (MainMenuClass)
	{
		if (HypercasualGameInstance->ShouldDisplayMainMenu)
		{
			UUserWidget* MainMenu = CreateWidget<UUserWidget, APlayerController*>(UGameplayStatics::GetPlayerController(GetWorld(), 0), MainMenuClass->GetAuthoritativeClass());
			MainMenu->AddToViewport();
			
			HypercasualGameInstance->ShouldDisplayMainMenu = false;
		}
		else
		{
			ABoulder* Boulder = Cast<ABoulder>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

			if (Boulder) Boulder->BeginPhysicsMovement();
		}
	}
}

// Called when player controller spawned
APlayerController* AHypercasualGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	APlayerController* PlayerController = Super::SpawnPlayerController(InRemoteRole, Options);

	if (ABoulderController* BoulderController = Cast<ABoulderController>(PlayerController))
	{
		// Spawn follow camera
		AActor* CameraActor = GetWorld()->SpawnActor(AFollowCamera::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);
		AFollowCamera* FollowCamera = Cast<AFollowCamera>(CameraActor);

		// Link camera to player controller
		FollowCamera->SetPlayerController(BoulderController);
		BoulderController->SetCamera(FollowCamera);
	}
	
	return PlayerController;
}

ATile* AHypercasualGameMode::SpawnNextTile()
{
	// Spawn transform for next tile
	if (LastTile) NextTileTransform = LastTile->GetAttachPointTransform();

	TArray<TSubclassOf<ATile>> TileArray;

	// Populate array of tiles based on their rarity
	for (const TPair<TSubclassOf<ATile>, ETileRarity> Element : TileFabs)
	{
		for (int32 i = 0; i < Element.Value + 1; i++)
		{
			TileArray.Add(Element.Key);
		}
	}

	// Randomly choose an index from the array
	int32 Index = FMath::RandRange(0, TileArray.Num()-1);

	if (LastTile)
	{
		// Recursively call this function to ensure tiles don't immediately repeat
		if (TileArray[Index]->GetAuthoritativeClass() == LastTile->GetClass())
		{
			return SpawnNextTile();
		}
	}

	// Spawn next tile from index in array
	ATile* NextTile = GetWorld()->SpawnActor<ATile>(TileArray[Index]->GetAuthoritativeClass(), NextTileTransform);
	
	if (NextTile)
	{
		// Once spawned, set to last tile
		NextTile->FinishSpawning(NextTileTransform);
		LastTile = NextTile;
	}
	
	return LastTile;
}

void AHypercasualGameMode::EndGame_Implementation(float DistanceTravelled, bool HasNewNighScore)
{
	if (HypercasualGameInstance)
	{
		if (HypercasualGameInstance->HighScore < FMath::TruncToInt32(DistanceTravelled))
		{
			HypercasualGameInstance->HighScore = FMath::TruncToInt32(DistanceTravelled);
			HypercasualGameInstance->SaveProfile();
		}
	}
	
}