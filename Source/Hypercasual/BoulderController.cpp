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

FVector ABoulderController::GetWorldLocationFromMousePosition() 
{
    FVector WorldLocationFromMousePosition;

    float MouseX, MouseY;
    GetMousePosition(MouseX, MouseY);

	const int32 MAX_TRACE_DIST = 5000;

    FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
    FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();
    FVector CameraDirection = CameraRotation.Vector().GetSafeNormal();

    FVector TraceStartLoc, TraceEndLoc;
    DeprojectScreenPositionToWorld(MouseX, MouseY, TraceStartLoc, CameraDirection);
    TraceEndLoc = TraceStartLoc + MAX_TRACE_DIST * CameraDirection;

    FHitResult* OutHit = new FHitResult();

    GetWorld()->LineTraceSingleByChannel(
        *OutHit,
        TraceStartLoc,
        TraceEndLoc,
        ECollisionChannel::ECC_Visibility
    );

    if (OutHit)
    {
        WorldLocationFromMousePosition = OutHit->Location;
    }

    return WorldLocationFromMousePosition;
}