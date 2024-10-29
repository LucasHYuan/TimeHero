// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Action.generated.h"

UCLASS()
class TIMEHERO_API AAction : public AActor
{
	GENERATED_BODY()
	
public:	
	AAction();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void ExecuteAction();
};
