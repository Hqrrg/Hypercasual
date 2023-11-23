// Fill out your copyright notice in the Description page of Project Settings.


#include "BoulderController.h"
#include "Kismet/KismetSystemLibrary.h"

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

FHitResult* ABoulderController::GetWorldLocationFromMousePosition() 
{
    float MouseX, MouseY;
    GetMousePosition(MouseX, MouseY);

	const int32 MAX_TRACE_DIST = 10000;

    FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
    FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();
    FVector CameraDirection = CameraRotation.Vector().GetSafeNormal();

    FVector TraceStartLoc, TraceEndLoc;
    DeprojectScreenPositionToWorld(MouseX, MouseY, TraceStartLoc, CameraDirection);
    TraceEndLoc = TraceStartLoc + MAX_TRACE_DIST * CameraDirection;

    FHitResult* OutHit = new FHitResult();
    FCollisionQueryParams QueryParams;
    QueryParams.bReturnPhysicalMaterial = true;

    bool LineTrace = GetWorld()->LineTraceSingleByChannel(
        *OutHit,
        TraceStartLoc,
        TraceEndLoc,
        ECC_Visibility,
        QueryParams
    );

    return OutHit->bBlockingHit ? OutHit : nullptr;
}