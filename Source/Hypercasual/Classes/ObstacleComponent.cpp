// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleComponent.h"
#include "Boulder.h"

UObstacleComponent::UObstacleComponent()
{
	SetBoundsScale(10.0f);
	BodyInstance.bNotifyRigidBodyCollision = true;
}

void UObstacleComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetCollisionObjectType(ECC_GameTraceChannel1);
	OnComponentHit.AddDynamic(this, &UObstacleComponent::OnHit);
}

void UObstacleComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	if (ABoulder* Boulder = Cast<ABoulder>(OtherActor))
	{
		Boulder->DecrementLives();
	} 
}
