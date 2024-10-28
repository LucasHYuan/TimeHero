// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.generated.h"

UCLASS()
class TIMEHERO_API AGridCell : public AActor
{
	GENERATED_BODY()
	
public:	
	AGridCell();
	AGridCell* ParentCell;
protected:
	virtual void BeginPlay() override;

private:

};
