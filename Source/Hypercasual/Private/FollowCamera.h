// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "FollowCamera.generated.h"

UCLASS()
class HYPERCASUAL_API AFollowCamera : public AActor
{
	GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Level", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CullingBox = nullptr;

public:
	// Sets default values for this actor's properties
	AFollowCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPlayerController(APlayerController* NewPlayerController);
	void SetFollowTarget(AActor* NewTarget);
	void SetOffset(float X, float Y, float Z);
	void SetPitch(float Pitch);

private:
	AActor* FollowTarget = nullptr;
	APlayerController* PlayerController = nullptr;
	FVector Offset = FVector(0.0f, 0.0f, 0.0f);
	
	UFUNCTION()
	void CullingBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};
