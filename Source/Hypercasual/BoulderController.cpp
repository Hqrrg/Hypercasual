// Fill out your copyright notice in the Description page of Project Settings.


#include "BoulderController.h"
#include "Kismet/KismetSystemLibrary.h"

ABoulderController::ABoulderController() 
{
    SetShowMouseCursor(true);
    SetInputMode(FInputModeGameOnly());
}

void ABoulderController::BeginPlay()
{
    Super::BeginPlay();
}

void ABoulderController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}