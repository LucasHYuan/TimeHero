// Fill out your copyright notice in the Description page of Project Settings.


#include "GridCursorManager.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Plane.h"

AGridCursorManager::AGridCursorManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGridCursorManager::UpdateCursorSelectionVisualization(FVector CursorLocation)
{
    float TileX = GridManager->TileSize.X;
    float TileY = GridManager->TileSize.Y;
    
    FVector DifferenceVec = CursorLocation - GridManager->LeftBottomPosition;
    FIntPoint NewCursorLocation = FIntPoint(FMath::RoundToInt(DifferenceVec.X / TileX),
                                            FMath::RoundToInt(DifferenceVec.Y / TileY));

    if (NewCursorLocation.X == LastCursorLocation.X && NewCursorLocation.Y == LastCursorLocation.Y)return;
    GridManager->UnSelected(LastCursorLocation);
    LastCursorLocation = NewCursorLocation;
    GridManager->Selected(NewCursorLocation);
}

void AGridCursorManager::BeginPlay()
{
	Super::BeginPlay();
    GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
}

void AGridCursorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CursorLocation;
	if (GetCursorLocationOnGrid(CursorLocation)) 
	{
        UpdateCursorSelectionVisualization(CursorLocation);
		DrawDebugSphere(GetWorld(), CursorLocation, 10.0f, 12, FColor::Green, false, -1.0f);
	}
}

bool AGridCursorManager::GetCursorLocationOnGrid(FVector& OutLocation)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        FHitResult HitResult;
        if (PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2), false, HitResult))
        {
            if (HitResult.bBlockingHit)
            {
                OutLocation = HitResult.Location;
                return true;
            }
        }
        else {
            FVector WorldLocation;
            FVector WorldDirection;
            PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
            FVector PlanePoint = GridManager->CenterPosition;
            FVector PlaneNormal = FVector(0, 0, 1);
            FVector IntersectionPoint;
            IntersectionPoint = FMath::LinePlaneIntersection(
                WorldLocation,
                WorldLocation + WorldDirection * 10000.0f,
                PlanePoint,
                PlaneNormal
            );
            if (bool bInDirection = FVector::DotProduct(IntersectionPoint - WorldLocation, WorldDirection) > 0) {
                OutLocation = IntersectionPoint;
            }
        }
    }
    return false;
}

