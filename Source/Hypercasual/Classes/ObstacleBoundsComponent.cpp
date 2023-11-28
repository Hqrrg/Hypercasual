// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleBoundsComponent.h"
#include "HypercasualGameMode.h"
#include "Boulder.h"

// Sets default values for this component's properties
UObstacleBoundsComponent::UObstacleBoundsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UObstacleBoundsComponent::BeginPlay()
{
	Super::BeginPlay();

	this->OnComponentBeginOverlap.AddDynamic(this, &UObstacleBoundsComponent::BeginOverlap);
}


// Called every frame
void UObstacleBoundsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UObstacleBoundsComponent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (ABoulder* Boulder = Cast<ABoulder>(OtherActor))
	{
		if (OtherComp == Boulder->BoulderMeshComponent)
		{
			if (AHypercasualGameMode* HypercasualGameMode = Cast<AHypercasualGameMode>(GetWorld()->GetAuthGameMode()))
			{
				HypercasualGameMode->EndGame();
			}
		}
	}
}
