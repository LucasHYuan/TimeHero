// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "Math/Vector.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/CollisionProfile.h"


AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = false;
	InstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMeshComponent"));
    InstancedMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
    InstancedMeshComponent->SetCollisionObjectType(ECC_EngineTraceChannel2);
    RootComponent = InstancedMeshComponent;
    GridInstanceIndex.SetNum(TileCount.X * TileCount.Y);
    for (int32 Row = 0; Row < TileCount.X; ++Row)
    {
        for (int32 Col = 0; Col < TileCount.Y; ++Col)
        {
            int32 Index = Row * TileCount.X + Col;
            GridInstanceIndex[Index] = nullptr;
        }
    }
}

void AGridManager::SetGridShapeType(EGridType type)
{
	Shape = type;
	GenerateGrid(type);
}



void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	GenerateGrid(Shape);
}

void AGridManager::ClearGrid()
{
	InstancedMeshComponent->ClearInstances();
}

void AGridManager::SnapToGrid(FVector& Vec1, FVector& Vec2)
{
    Vec1 = FVector(FMath::RoundToInt(Vec1.X / Vec2.X) * Vec2.X,
                   FMath::RoundToInt(Vec1.Y / Vec2.Y) * Vec2.Y,
                   Vec1.Z);
    if (Shape == EGridType::Square && TileCount.X % 2 != 0)Vec1.X += Vec2.X / 2;
    if (Shape == EGridType::Square && TileCount.Y % 2 != 0)Vec1.Y += Vec2.Y / 2;

}

void AGridManager::Selected(int32 x, int32 y)
{
    int32 idx = x * TileCount.X + y;
    Selected(idx);
}

void AGridManager::Selected(int32 idx)
{
    if (idx<0 || idx>TileCount.X * TileCount.Y - 1)return;
    int32 MeshIndex;
    if (GridInstanceIndex[idx])MeshIndex = GridInstanceIndex[idx]->MeshIdx;
    else return;
    GridInstanceIndex[idx]->Selected();
}

void AGridManager::Selected(FIntPoint point)
{
    Selected(point.X, point.Y);
}

void AGridManager::UnSelected(int32 x, int32 y)
{
    int32 idx = x * TileCount.X + y;
    UnSelected(idx);
}

void AGridManager::UnSelected(int32 idx)
{
    if (idx<0 || idx>TileCount.X * TileCount.Y - 1)return;
    int32 MeshIndex;
    if (GridInstanceIndex[idx])MeshIndex = GridInstanceIndex[idx]->MeshIdx;
    else return;
    GridInstanceIndex[idx]->UnSelected();
    
}

void AGridManager::UnSelected(FIntPoint point)
{
    UnSelected(point.X, point.Y);
}

void AGridManager::Clicked(int32 x, int32 y)
{
    int32 idx = x * TileCount.X + y;
    Clicked(idx);
}

void AGridManager::Clicked(int32 idx) {
    if (idx<0 || idx>TileCount.X * TileCount.Y - 1)return;
    int32 MeshIndex;
    if (GridInstanceIndex[idx])MeshIndex = GridInstanceIndex[idx]->MeshIdx;
    else return;
    GridInstanceIndex[idx]->Clicked();
    if(Source==-1)Source = idx;
}

void AGridManager::Clicked(FIntPoint point)
{
    Clicked(point.X, point.Y);
}

TArray<AGridCell*> AGridManager::FindPath(AGridCell* StartCell, AGridCell* EndCell)
{
    TArray<AGridCell*> Path;

    if (!StartCell || !EndCell)
    {
        if(!StartCell)
        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, "Path Find Failed Start Cell");
        if (!EndCell)
        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, "Path Find Failed End Cell");
        return Path;
    }

    TSet<AGridCell*> OpenSet;
    OpenSet.Add(StartCell);

    TSet<AGridCell*> ClosedSet;

    TMap<AGridCell*, float> GScore;
    GScore.Add(StartCell, 0.0f);

    TMap<AGridCell*, float> FScore;
    /*HeuristicCost(StartCell, EndCell)*/
    // Mahattan Distance Here
    FScore.Add(StartCell, FMath::Abs(StartCell->X - EndCell->X) + FMath::Abs(StartCell->Y - EndCell->Y));

    TMap<AGridCell*, AGridCell*> CameFrom;

    while (OpenSet.Num() > 0)
    {
        AGridCell* CurrentCell = nullptr;
        float LowestFScore = FLT_MAX;
        for (AGridCell* Cell : OpenSet)
        {
            float Score = FScore.Contains(Cell) ? FScore[Cell] : FLT_MAX;
            if (Score < LowestFScore)
            {
                LowestFScore = Score;
                CurrentCell = Cell;
            }
        }

        if (CurrentCell == EndCell)
        {
            while (CurrentCell)
            {
                Path.Insert(CurrentCell, 0);
                CurrentCell = CameFrom.Contains(CurrentCell) ? CameFrom[CurrentCell] : nullptr;
            }
            return Path;
        }

        OpenSet.Remove(CurrentCell);
        ClosedSet.Add(CurrentCell);

        for (AGridCell* Neighbor : GetValidNeighbours(CurrentCell))
        {
            if (ClosedSet.Contains(Neighbor))
            {
                continue;
            }

            float TentativeGScore = GScore[CurrentCell] + (CurrentCell->Cost);

            if (!OpenSet.Contains(Neighbor))
            {
                OpenSet.Add(Neighbor);
            }
            else if (TentativeGScore >= GScore[Neighbor])
            {
                continue;
            }

            CameFrom.Add(Neighbor, CurrentCell);
            GScore.Add(Neighbor, TentativeGScore);
            FScore.Add(Neighbor, TentativeGScore + FMath::Abs(Neighbor->X - EndCell->X) + FMath::Abs(Neighbor->Y - EndCell->Y));
        }
    }

    return Path;
}

TArray<AGridCell*> AGridManager::GetValidNeighbours(AGridCell* Cell)
{
    TArray<AGridCell*> Neighbours;

    if (!Cell)
    {
        return Neighbours;
    }

    int32 X = Cell->X;
    int32 Y = Cell->Y;

    TArray<FIntPoint> Directions = {
        FIntPoint(-1, 0),
        FIntPoint(1, 0),
        FIntPoint(0, -1),
        FIntPoint(0, 1)
    };

    for (const FIntPoint& Dir : Directions)
    {
        int32 NeighbourRow = X + Dir.X;
        int32 NeighbourColumn = Y + Dir.Y;

        if (NeighbourRow >= 0 && NeighbourRow < TileCount.X &&
            NeighbourColumn >= 0 && NeighbourColumn < TileCount.Y)
        {
            int32 Index = NeighbourRow * TileCount.Y + NeighbourColumn;
            AGridCell* Neighbour = GridInstanceIndex[Index];
            if (Neighbour)
            {
                Neighbours.Add(Neighbour);
            }
        }
    }

    return Neighbours;
}

void AGridManager::SetTarget(FIntPoint point)
{   
    int32 idx = point.X * TileCount.X + point.Y;
    if (idx<0 || idx>TileCount.X * TileCount.Y - 1)return;
    int32 MeshIndex;
    if (GridInstanceIndex[idx])MeshIndex = GridInstanceIndex[idx]->MeshIdx;
    else return;
    Target = idx;
    /*TArray<AGridCell*> Neighbours = GetValidNeighbours(GridInstanceIndex[Target]);
    for (auto GridCell : Neighbours) {
        GridCell->OnPath();
    }*/
    if (Target == -1 || Source == -1)return;
    TArray<AGridCell*> Path = FindPath(GridInstanceIndex[Source], GridInstanceIndex[Target]);
    for (auto GridCell : Path) {
        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, "1");
        GridCell->OnPath();
    }
}




void AGridManager::GenerateGrid(EGridType type)
{
	if (!DataTable) {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "ERROR::GridManager::NO DATATABLE");
		return;
	}
	ClearGrid();
    FName RowName = FName(*UEnum::GetDisplayValueAsText(Shape).ToString());
    GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, RowName.ToString());
    FMeshData* MeshData = DataTable->FindRow<FMeshData>(RowName, "MeshData Lookup");
    if (MeshData)
    {
        InstancedMeshComponent->SetStaticMesh(MeshData->FlatMesh);
        InstancedMeshComponent->SetMaterial(0, MeshData->FlatBorderMaterial);
        TileSize = MeshData->MeshSize;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Row not found in DataTable for %s"), *RowName.ToString());
    }

    FVector MeshBounds = InstancedMeshComponent->GetStaticMesh()->GetBoundingBox().GetSize();
    FVector Scale = FVector(TileSize.X / MeshBounds.X, TileSize.Y / MeshBounds.Y, 1.0f);
    FTransform InstanceTransform;
    InstanceTransform.SetScale3D(Scale);
    SnapToGrid(CenterPosition, TileSize);

    LeftBottomPosition = CenterPosition - FVector(TileSize.X * TileCount.X/2, TileSize.Y * TileCount.Y/2, 0.0f);
    UWorld* World = GetWorld();
    float SphereRadius = TileSize.Z / 3.0;
    FCollisionShape MySphere = FCollisionShape::MakeSphere(SphereRadius);
    for (int32 x = 0; x < TileCount.X; ++x) {
        for (int32 y = 0; y < TileCount.Y; ++y) {
            FVector Location;
            
            switch (Shape) {
            case EGridType::Square:
                Location = LeftBottomPosition + FVector(x * TileSize.X, y * TileSize.Y, 0.0f);
                break;

            case EGridType::Hexagon:
                Location = LeftBottomPosition + FVector(
                    x * TileSize.X * 0.75f,
                    y * TileSize.Y + (x % 2 == 0 ? 0 : TileSize.Y * 0.5f),
                    0.0f
                );
                break;
            // TODO - NOT CORRECT
            case EGridType::Triangle:
                Location = LeftBottomPosition + FVector(
                    x * TileSize.X,
                    y * TileSize.Y * (x % 2 == 0 ? 1.0f : 0.5f),
                    0.0f
                );
                break;
            }

            TArray<FHitResult> OutHits;
            bool bHit = GetWorld()->SweepMultiByChannel(OutHits, Location + FVector(0, 0, 2000), Location, FQuat::Identity, ECC_GameTraceChannel1, MySphere);

            if (bHit)
            {
                // TODO - Maybe Better API to get the first hit item?
                for (const FHitResult& Hit : OutHits)
                {
                    FVector GroundLocation = Hit.Location;
                    DrawDebugSphere(World, GroundLocation, TileSize.X / 3.0f, 12, FColor::Blue, false, 1000.0f);
                    Location.Z = FMath::RoundToInt((GroundLocation.Z - SphereRadius)/TileSize.Z) * TileSize.Z  + 3;
                    break;
                }
                InstanceTransform.SetLocation(Location);
                AGridCell* NewCell = GetWorld()->SpawnActor<AGridCell>(AGridCell::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
                GridInstanceIndex[x * TileCount.Y + y] = NewCell;
                NewCell->MeshIdx = InstancedMeshComponent->AddInstance(InstanceTransform);
                NewCell->X = x;
                NewCell->Y = y;
                NewCell->SetInstancedStaticMeshComponent(InstancedMeshComponent);
            }
            
        }
    }
    
}

