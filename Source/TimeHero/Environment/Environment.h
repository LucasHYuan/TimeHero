// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Environment.generated.h"

UCLASS()
class TIMEHERO_API AEnvironment : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnvironment();

protected:
	virtual void BeginPlay() override;


};
