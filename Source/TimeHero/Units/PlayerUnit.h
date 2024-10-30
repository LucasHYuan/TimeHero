// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUnit.h"
#include "Animation/AnimInstance.h"
#include "PlayerUnit.generated.h"

/**
 * 
 */
//class ABP_Warrior;

UCLASS()
class TIMEHERO_API APlayerUnit : public ABaseUnit
{
	GENERATED_BODY()
	
public:
	APlayerUnit();

	virtual void MoveToTile(FIntPoint Destination) override;
	virtual void Attack(ABaseUnit* TargetUnit) override;
	virtual void TakeDamage(float DamageAmount) override;

protected:
    virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UPlayerAnimInstance* PlayerAnimInstance;

    void UpdateAnimationState();

	bool bIsMoving;
	bool bIsAttacking;
	bool bIsHit;
	bool bIsDead;
	int32 X;
	int32 Y;
};
