// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Private\BoulderNiagaraActor.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
ABoulderNiagaraActor::ABoulderNiagaraActor()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void ABoulderNiagaraActor::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle UpdateLocationTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(UpdateLocationTimerHandle, this, &ABoulderNiagaraActor::UpdateActorLocation, 0.001f, true);
	
}

void ABoulderNiagaraActor::SetFollowActor(AActor* Actor)
{
	FollowActor = Actor;
}

void ABoulderNiagaraActor::SpawnNiagaraSystem(UNiagaraSystem* NiagaraSystem, float Duration)
{
	if (IsDisplayingNiagaraSystem(NiagaraSystem)) return;
	
	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, SceneComponent, NAME_None, SceneComponent->GetComponentLocation(), SceneComponent->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true, true);

	if (NiagaraComponent)
	{
		NiagaraComponent->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
		ActiveNiagaraSystems.Add(NiagaraSystem);
	}
	
	FTimerHandle DestroyNiagaraComponentHandle;
	FTimerDelegate DestroyNiagaraComponentDelegate;
	DestroyNiagaraComponentDelegate.BindUFunction(this, FName("DestroyNiagaraComponent"), NiagaraComponent);

	GetWorld()->GetTimerManager().SetTimer(DestroyNiagaraComponentHandle, DestroyNiagaraComponentDelegate, Duration, false);
}

bool ABoulderNiagaraActor::IsDisplayingNiagaraSystem(UNiagaraSystem* NiagaraSystem)
{
	return ActiveNiagaraSystems.Contains(NiagaraSystem);
}

void ABoulderNiagaraActor::UpdateActorLocation()
{
	if (FollowActor)
	{
		FVector FollowActorLocation = FollowActor->GetActorLocation();
		FVector NewActorLocation = FVector(FollowActorLocation.X, FollowActorLocation.Y, GetActorLocation().Z);
		SetActorLocation(NewActorLocation);
	}
}

void ABoulderNiagaraActor::DestroyNiagaraComponent(UNiagaraComponent* NiagaraComponent)
{
	UNiagaraSystem* NiagaraSystem = NiagaraComponent->GetAsset();
	
	if (IsDisplayingNiagaraSystem(NiagaraSystem))
	{
		ActiveNiagaraSystems.Remove(NiagaraSystem);
	}
	NiagaraComponent->UnregisterComponent();
	NiagaraComponent->DestroyComponent();
}

