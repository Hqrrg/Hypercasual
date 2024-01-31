// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleStaticMeshComponent.h"
#include "Boulder.h"

#define OBSTACLE_COLLISION_CHANNEL ECC_EngineTraceChannel1

void UObstacleStaticMeshComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	SetCollisionObjectType(OBSTACLE_COLLISION_CHANNEL);
	SetGenerateOverlapEvents(true);
	
	OnComponentBeginOverlap.AddDynamic(this, &UObstacleStaticMeshComponent::OnBeginOverlap);
}

void UObstacleStaticMeshComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABoulder* Boulder = Cast<ABoulder>(OtherActor))
	{
		SetGenerateOverlapEvents(false);
		
		if (!Boulder->Kill() && OverlappedMaterial) SetMaterial(0, OverlappedMaterial);
	}
}

