// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/Pawn.h"
#include "Barrier.generated.h"

USTRUCT(BlueprintType)
struct FBarrierInfo : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material = nullptr;
};

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

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	void SetUpgraded(bool IsUpgraded);
	FORCEINLINE bool IsUpgraded() { return Upgraded; }
	void AddNextPoint();
	void Decay();
	
	FTimerHandle DecayTimerHandle;

private:
	UPROPERTY()
	TArray<USplineMeshComponent*> BarrierMeshComps;
	
	int32 MaxSplineLength = 150;
	FVector LastPointPosition;
	bool Upgraded = false;

	bool UpdateSplinePoints(FVector PointA, FVector PointB, float DesiredDistance);
	void AddMeshComponents();

	UPROPERTY()
	UDataTable* BarrierDataTable = nullptr;
	
	FBarrierInfo* BarrierInfo = nullptr;

	void LoadAssets(FName RowIdentifier);
	void LoadAssets();
};
