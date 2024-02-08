// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "ObstacleStaticMeshComponent.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FObstacleInfo : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material = nullptr;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* DisabledMaterial = nullptr;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* PassthroughMaterial = nullptr;
	
};

UCLASS(ClassGroup = ("Custom"), meta = (BlueprintSpawnableComponent))
class HYPERCASUAL_API UObstacleStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* ObstacleMaterial = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* DisabledMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* PassthroughMaterial = nullptr;

public:
	UObstacleStaticMeshComponent();
	
protected:
	virtual void OnComponentCreated() override;

public:
	void SetPassthrough(bool Passthrough);
	bool IsPassthrough = false;
	
private:
	bool Disabled = false;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	UDataTable* ObstacleInfoDataTable = nullptr;
	
	FObstacleInfo* ObstacleInfo = nullptr;
};
