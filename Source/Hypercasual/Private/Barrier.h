// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/Pawn.h"
#include "Barrier.generated.h"

UCLASS()
class HYPERCASUAL_API ABarrier : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "", meta = (AllowPrivateAccess = "true"))
	USplineComponent* BarrierSpline = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* BarrierMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (AllowPrivateAccess = "true"));
	UMaterialInterface* BarrierMaterial = nullptr;
	
public:	
	// Sets default values for this actor's properties
	ABarrier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddNextPoint();

private:
	int32 MAX_SPLINE_LENGTH = 1500;
	TArray<USplineMeshComponent*> BarrierMeshComps;
	FTimerHandle DecayTimerHandle;

	void AddMeshComponents();
	void Decay();
};
