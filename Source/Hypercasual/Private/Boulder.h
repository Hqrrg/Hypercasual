// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Barrier.h"
#include "BoulderNiagaraActor.h"
#include "EnhancedInputSubsystemInterface.h"
#include "HypercasualGameMode.h"
#include "InputAction.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/Pawn.h"
#include "Boulder.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateLivesDelegate, int32, Lives);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickup, FPickupInfo, PickupInfo);

UCLASS()
class HYPERCASUAL_API ABoulder : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* BoulderMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BoulderMaterial = nullptr;

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
	
	FORCEINLINE int32 GetRemainingLives() { return RemainingLives; };
	FORCEINLINE int32 GetLives() { return Lives; }
	
	void SetRemainingLives(int32 NewRemainingLives);
	
	UFUNCTION()
	void ToggleImmunity(bool Damaged = true, bool ForceImmune = false, float ImmunityDuration = 3.0f);
	
	bool Kill();

	UFUNCTION(BlueprintCallable)
	void BeginPhysicsMovement();
	
	void StopPhysicsMovement();

	FORCEINLINE float GetVelocityLimit() { return VelocityLimit; }
	FORCEINLINE float GetAcceleration() { return Acceleration; }
	FORCEINLINE void SetVelocityLimit(float NewLimit) { VelocityLimit = NewLimit; }
	FORCEINLINE void SetAcceleration(float NewAcceleration) { Acceleration = NewAcceleration; }

	UFUNCTION()
	void ToggleTemporaryVelocityBoost(float Duration, bool ForceBoost);

	UFUNCTION()
	void SetUpgradedBarrier(float Duration, bool IsUpgraded);

	UPROPERTY(BlueprintAssignable)
	FUpdateLivesDelegate OnUpdateLives;

	UPROPERTY(BlueprintAssignable)
	FPickup OnPickup;
	
	bool Immune = false;
	bool IsVelocityBoosted = false;

	UPROPERTY()
	ABoulderNiagaraActor* BoulderNiagaraActor = nullptr;

	void CancelBuild(const FInputActionValue& Value);
	
private:
	UPROPERTY()
	AHypercasualGameMode* HypercasualGameMode = nullptr;
	
	void Build(const FInputActionValue& Value);

	FTimerHandle BuildTimerHandle;
	FTimerHandle MovementTimerHandle;
	FTimerHandle ResetImmunityTimerHandle;
	FTimerHandle ToggleVisibilityTimerHandle;
	FTimerHandle TemporaryVelocityBoostTimerHandle;
	FTimerHandle BrakeTimerHandle;
	FTimerHandle UpgradedBarrierTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	float Acceleration = 200.0f;

	UPROPERTY()
	float DefaultAcceleration = Acceleration;

	UPROPERTY(EditDefaultsOnly, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	float VelocityBoostAcceleration = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	float VelocityLimit = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	int32 VelocityIncreaseInterval = 500;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DistanceTravelled = 0.0f;
	
	float SpawningLocationX = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	int32 Lives = 3;

	ABarrier* CurrentBarrier = nullptr;
	int32 RemainingLives = Lives;
	int32 LastVelocityIncreaseInterval = 0;
	float CachedAcceleration = 0.0f;
	float CachedVelocityLimit = 0.0f;
	bool HasUpgradedBarrier = false;
	
	UFUNCTION()
	void ShiftWorldOrigin();

	UFUNCTION()
	void Move();

	UFUNCTION()
	void ToggleVisibility();

	UFUNCTION()
	void Brake();
};
