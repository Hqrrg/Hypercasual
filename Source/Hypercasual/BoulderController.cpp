// Fill out your copyright notice in the Description page of Project Settings.


#include "BoulderController.h"
#include "Kismet/KismetSystemLibrary.h"

//Defining custom trace channel
#define BARRIER_TRACE_CHANNEL ECollisionChannel::ECC_GameTraceChannel1

ABoulderController::ABoulderController() 
{
    SetShowMouseCursor(true);
    //Can't set FInputMode (Used to be able to!) -> Causes project to have access memory violations - tried in begin play too and causes other issues. Unreal engine is a piece of shit.
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
    FVector WorldLocationFromMousePosition = FVector(0.0f,0.0f,0.0f);

    float MouseX, MouseY;
    GetMousePosition(MouseX, MouseY);

	const int32 MAX_TRACE_DIST = 5000;

    FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
    FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();
    FVector CameraDirection = CameraRotation.Vector().GetSafeNormal();

    FVector TraceStartLoc, TraceEndLoc;
    DeprojectScreenPositionToWorld(MouseX, MouseY, TraceStartLoc, CameraDirection);
    TraceEndLoc = TraceStartLoc + MAX_TRACE_DIST * CameraDirection;

    TArray<FHitResult> OutHits;

    bool LineTrace = GetWorld()->LineTraceMultiByChannel(
        OutHits,
        TraceStartLoc,
        TraceEndLoc,
        BARRIER_TRACE_CHANNEL
    );

    if (LineTrace && FHitResult::GetNumOverlapHits(OutHits) == 0)
    {
        for (FHitResult OutHit : OutHits)
        {
            WorldLocationFromMousePosition = OutHit.Location;
        }
    }
    return WorldLocationFromMousePosition;
}