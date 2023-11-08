// Copyright Epic Games, Inc. All Rights Reserved.

#include "HypercasualGameMode.h"
#include "HypercasualCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHypercasualGameMode::AHypercasualGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
