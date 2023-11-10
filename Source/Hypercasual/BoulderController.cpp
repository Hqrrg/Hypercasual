// Fill out your copyright notice in the Description page of Project Settings.


#include "BoulderController.h"

ABoulderController::ABoulderController() 
{
    SetShowMouseCursor(true);
    SetInputMode(FInputModeGameOnly());
}

void ABoulderController::BeginPlay()
{
    Super::BeginPlay();
}