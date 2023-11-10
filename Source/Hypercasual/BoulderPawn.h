// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "BoulderPawn.generated.h"

UCLASS(config=Game)
class HYPERCASUAL_API ABoulderPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoulderPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoulderMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FollowCamera = nullptr;

};
