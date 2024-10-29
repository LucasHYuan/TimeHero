// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridManager.h"
#include "GridCursorManager.generated.h"

UCLASS()
class TIMEHERO_API UCursorComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UCursorComponent();
	void UpdateCursorSelectionVisualization(FVector CursorLocation);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	void HandleTick(float DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetCursorLocationOnGrid(FVector& OutLocation);

	UFUNCTION()
	void OnLeftMouseClick();

	UFUNCTION()
	void OnRightMouseClick();
private:
	FIntPoint LastCursorLocation;
	AGridManager* GridManager;
};
