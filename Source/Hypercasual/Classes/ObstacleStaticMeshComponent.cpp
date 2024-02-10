// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleStaticMeshComponent.h"
#include "Boulder.h"

#define OBSTACLE_COLLISION_CHANNEL ECC_EngineTraceChannel1

UObstacleStaticMeshComponent::UObstacleStaticMeshComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ObstacleInfoDataTableAsset(TEXT("/Game/Hypercasual/DataTables/DT_ObstacleInfo.DT_ObstacleInfo"));

	if (ObstacleInfoDataTableAsset.Succeeded())
	{
		ObstacleInfoDataTable = ObstacleInfoDataTableAsset.Object;
	}

	if (ObstacleInfoDataTable)
	{
		static const FString ContextString(TEXT("Obstacle Info Context"));
		ObstacleInfo = ObstacleInfoDataTable->FindRow<FObstacleInfo>(FName("Default"), ContextString, true);
	}

	SetBoundsScale(5.0f);
}

void UObstacleStaticMeshComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (ObstacleInfo)
	{
		ObstacleMaterial = ObstacleInfo->Material;
		DisabledMaterial = ObstacleInfo->DisabledMaterial;
		PassthroughMaterial = ObstacleInfo->PassthroughMaterial;
	}
	
	if (ObstacleMaterial) SetMaterial(0, ObstacleMaterial);
	
	SetCollisionObjectType(OBSTACLE_COLLISION_CHANNEL);
	SetGenerateOverlapEvents(true);
	
	OnComponentBeginOverlap.AddDynamic(this, &UObstacleStaticMeshComponent::OnBeginOverlap);
}


void UObstacleStaticMeshComponent::SetPassthrough(bool Passthrough)
{
	IsPassthrough = Passthrough;
	
	if (!Disabled && PassthroughMaterial)
	{
		if (Passthrough)
		{
			SetMaterial(0, PassthroughMaterial);
		}
		else
		{
			SetMaterial(0, ObstacleMaterial);
		}
		SetGenerateOverlapEvents(!Passthrough);
	}
}

void UObstacleStaticMeshComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABoulder* Boulder = Cast<ABoulder>(OtherActor))
	{
		Disabled = true;
		SetGenerateOverlapEvents(false);
		
		if (!Boulder->Kill() && DisabledMaterial) SetMaterial(0, DisabledMaterial);
	}
}

