// Copyright Epic Games, Inc. All Rights Reserved.

#include "HypercasualGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Tile.h"
#include "BoulderController.h"
#include "FollowCamera.h"

AHypercasualGameMode::AHypercasualGameMode()
{
	NextTileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
}
 
void AHypercasualGameMode::BeginPlay()
{
	Super::BeginPlay();
	for (int32 i = 0; i < 4; i++)
	{
		SpawnNextTile();
	}
}

APlayerController* AHypercasualGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	APlayerController* PlayerController = Super::SpawnPlayerController(InRemoteRole, Options);

	if (ABoulderController* BoulderController = Cast<ABoulderController>(PlayerController))
	{
		UWorld* World = GetWorld();
		AActor* CameraActor = World->SpawnActor(AFollowCamera::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);
	
		AFollowCamera* FollowCamera = Cast<AFollowCamera>(CameraActor);
		FollowCamera->SetPlayerController(PlayerController);
	
		BoulderController->SetCamera(FollowCamera);	
	}
	
	return PlayerController;
	
}

void AHypercasualGameMode::SpawnNextTile()
{
	if (LastTile) NextTileTransform = LastTile->GetAttachPointTransform();

	TArray<TSubclassOf<ATile>> TileArray;

	for (const TPair<TSubclassOf<ATile>, TileRarity> Element : TileFabs)
	{
		for (int32 i = 0; i < Element.Value + 1; i++)
		{
			TileArray.Add(Element.Key);
		}
	}

	int32 Index = FMath::RandRange(0, TileArray.Num()-1);
	ATile* NextTile = GetWorld()->SpawnActor<ATile>(TileArray[Index]->GetAuthoritativeClass(), NextTileTransform);
	
	if (NextTile)
	{
		NextTile->FinishSpawning(NextTileTransform);
		LastTile = NextTile;
	}
}

void AHypercasualGameMode::EndGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString("Game Over!"));
}