// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "BoulderNiagaraActor.generated.h"

UCLASS()
class HYPERCASUAL_API ABoulderNiagaraActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Appearance")
	USceneComponent* SceneComponent = nullptr;

public:
	// Sets default values for this actor's properties
	ABoulderNiagaraActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetFollowActor(AActor* Actor);
	void SpawnNiagaraSystem(UNiagaraSystem* NiagaraSystem, float Duration);

private:
	UPROPERTY()
	TArray<UNiagaraSystem*> ActiveNiagaraSystems;

	bool IsDisplayingNiagaraSystem(UNiagaraSystem* NiagaraSystem);
	
	UPROPERTY()
	AActor* FollowActor = nullptr;

	UFUNCTION()
	void UpdateActorLocation();

	UFUNCTION()
	void DestroyNiagaraComponent(UNiagaraComponent* NiagaraComponent);
};
