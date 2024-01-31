// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ObstacleStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = ("Custom"), meta = (BlueprintSpawnableComponent))
class HYPERCASUAL_API UObstacleStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* OverlappedMaterial = nullptr;

protected:
	virtual void OnComponentCreated() override;
	
private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
