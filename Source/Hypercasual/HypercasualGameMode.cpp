// Copyright Epic Games, Inc. All Rights Reserved.

#include "HypercasualGameMode.h"
#include "BoulderPawn.h"
#include "BoulderController.h"
#include "UObject/ConstructorHelpers.h"
#include "Tile.h"

AHypercasualGameMode::AHypercasualGameMode()
{
	//Set default controller class
	PlayerControllerClass = ABoulderController::StaticClass();
	//Set default pawn class
	DefaultPawnClass = GamemodeDefaultPawnClass;
	//Set default tile transform 
	NextTileTransform = FTransform(FRotator(-10.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
}

void AHypercasualGameMode::BeginPlay() 
{
	Super::BeginPlay();

	for (int32 i = 0; i < 3; i++)
	{
		SpawnNextTile();
	}
}

void AHypercasualGameMode::SpawnNextTile() 
{
	ATile* NextTile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), NextTileTransform);
	
	if (IsValid(NextTile))
	{
		NextTile->FinishSpawning(NextTileTransform);
		NextTileTransform = NextTile->GetAttachPointTransform();
	}
}