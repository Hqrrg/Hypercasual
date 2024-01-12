// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleComponent.h"
#include "Boulder.h"

UObstacleComponent::UObstacleComponent()
{
	SetBoundsScale(10.0f);
	SetCollisionObjectType(ECC_GameTraceChannel1);
	BodyInstance.bNotifyRigidBodyCollision = true;
}

void UObstacleComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetCollisionObjectType(ECC_GameTraceChannel1);
	OnComponentBeginOverlap.AddDynamic(this, &UObstacleComponent::OnBeginOverlap);
}

void UObstacleComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABoulder* Boulder = Cast<ABoulder>(OtherActor))
	{
		Boulder->DecrementLives();
	}
}
