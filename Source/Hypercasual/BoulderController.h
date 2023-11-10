// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BoulderController.generated.h"

/**
 * 
 */
UCLASS()
class HYPERCASUAL_API ABoulderController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABoulderController();

protected:
	virtual void BeginPlay() override;
};



