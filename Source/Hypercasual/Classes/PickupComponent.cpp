// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"
#include "Boulder.h"


UPickupComponent::UPickupComponent()
{
}

void UPickupComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	this->OnComponentBeginOverlap.AddDynamic(this, &UPickupComponent::BeginOverlap);
}

void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPickupComponent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABoulder* Boulder = Cast<ABoulder>(OtherActor))
	{
		DestroyComponent();
	}
}
