// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class HYPERCASUAL_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform &Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TileMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	UStaticMesh* TileMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* AttachPoint = nullptr;

	FTransform GetAttachPointTransform();
};
