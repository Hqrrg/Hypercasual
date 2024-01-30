// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FollowCamera.h"
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
	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetCamera(AFollowCamera* NewCamera);
	FHitResult* GetHitFromMousePosition();

private:
	AFollowCamera* FollowCamera = nullptr;
};



