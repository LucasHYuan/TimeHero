// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseUnit.generated.h"

UCLASS()
class TIMEHERO_API ABaseUnit : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseUnit();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float AttackPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float DefensePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float RemainingTime;

    UFUNCTION(BlueprintCallable, Category = "Actions")
    virtual void MoveToTile(FIntPoint Destination);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    virtual void Attack(ABaseUnit* TargetUnit);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    virtual void TakeDamage(float DamageAmount);

protected:
	virtual void BeginPlay() override;

public:	

};
