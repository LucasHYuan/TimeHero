// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "../Grids/GridCell.h"
#include "GridManager.generated.h"

UENUM(BlueprintType)
enum class EGridType : uint8
{
	Square UMETA(DisplayName = "Square"),
	Triangle UMETA(DisplayName = "Triangle"),
	Hexagon UMETA(DisplayName = "Hexagon")
};

USTRUCT(BlueprintType)
struct FMeshData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	FVector MeshSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UMaterialInstance* MeshMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UStaticMesh* FlatMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UMaterialInstance* FlatBorderMaterial;
};

UCLASS()
class TIMEHERO_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGridManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	UDataTable* DataTable;


	UPROPERTY(EditAnywhere, Category = "Grid")
	EGridType Shape = EGridType::Square;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector TileSize = FVector(100, 100, 100);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FIntPoint TileCount = FIntPoint(100,100);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector CenterPosition = AActor::GetActorLocation();

	FVector LeftBottomPosition;
	int32 Source = -1;
	int32 Target = -1;
	// FUNCTIONS //

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void GenerateGrid(EGridType type);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetGridShapeType(EGridType type);

	void SnapToGrid(FVector& Vec1, FVector& Vec2);
	void Selected(int32 x, int32 y);
	void Selected(int32 idx);
	void Selected(FIntPoint point);
	void UnSelected(int32 x, int32 y);
	void UnSelected(int32 idx);
	void UnSelected(FIntPoint point);
	void Clicked(int32 x, int32 y);
	void Clicked(int32 idx);
	void Clicked(FIntPoint point);
	TArray<AGridCell*> FindPath(AGridCell* StartCell, AGridCell* EndCell);
	TArray<AGridCell*> GetValidNeighbours(AGridCell* Cell);
	void SetTarget(FIntPoint point);
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Grid")
	UInstancedStaticMeshComponent* InstancedMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Grid")
	TArray<AGridCell*> GridInstanceIndex;

	void ClearGrid();
	
};
