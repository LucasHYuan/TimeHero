// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.h"
#include "GridCursorManager.generated.h"

UCLASS()
class TIMEHERO_API AGridCursorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridCursorManager();
	void UpdateCursorSelectionVisualization(FVector CursorLocation);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetCursorLocationOnGrid(FVector& OutLocation);

private:
	FIntPoint LastCursorLocation;
	AGridManager* GridManager;
};
