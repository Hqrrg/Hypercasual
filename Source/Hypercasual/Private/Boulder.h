// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "InputAction.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/Pawn.h"
#include "Boulder.generated.h"

UCLASS()
class HYPERCASUAL_API ABoulder : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* BoulderMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BoulderMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BoulderImmuneBaseMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BoulderImmuneOverlayMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* BuildAction = nullptr;
	
public:
	// Sets default values for this actor's properties
	ABoulder();

	UPROPERTY(VisibleAnywhere, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BoulderMeshComponent = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	int32 SetMaxLinearVelocity(int32 Velocity);
	int32 GetRemainingLives();
	void DecrementLives();
	void EndGame();

private:
	void Build(const FInputActionValue& Value);
	void CancelBuild(const FInputActionValue& Value);

	FTimerHandle BuildTimerHandle;
	FTimerHandle PhysicsMovementHandle;
	FTimerHandle ResetGhostedTimerHandle;
	FTimerHandle FlickerTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	int32 MaxLinearVelocity = 2500;
	
	int32 DistanceTravelled = 0;
	int32 SpawningLocationX = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	int32 Lives = 3;
	
	int32 RemainingLives = Lives;

	bool Ghosted = false;
	
	UFUNCTION()
	void ShiftWorldOrigin();

	UFUNCTION()
	void ApplyPhysicsMovement();

	UFUNCTION()
	void Ghost();

	UFUNCTION()
	void ToggleGhostMaterialOverlay();
};
