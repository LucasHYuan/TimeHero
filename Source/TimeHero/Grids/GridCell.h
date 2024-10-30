// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Units/BaseUnit.h"
#include "../Units/PlayerUnit.h"
#include "GridCell.generated.h"

UCLASS()
class TIMEHERO_API AGridCell : public AActor
{
	GENERATED_BODY()
	
public:	
	AGridCell();
	AGridCell(int32 idx, int32 x, int32 y) :MeshIdx(idx), X(x), Y(y) {};
	AGridCell* ParentCell;
	int32 MeshIdx;
	int32 X;
	int32 Y;
	bool bIsClicked = false;
	float Cost = 1.0f;

	void SetInstancedStaticMeshComponent(UInstancedStaticMeshComponent* IMC);
	void Selected();
	void UnSelected();
	void Clicked();
	void OnPath();
	void SetPos(int32 x, int32 y);
protected:
	virtual void BeginPlay() override;
	ABaseUnit* Unit;
private:
	UInstancedStaticMeshComponent* InstancedMeshComponent;
	void GeneratePlayer();
};
