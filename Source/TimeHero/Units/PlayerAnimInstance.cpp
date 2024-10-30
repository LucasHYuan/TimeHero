// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

void UPlayerAnimInstance::UpdateAnimationStates(bool IsMoving, bool IsAttacking, bool IsHit, bool IsDead)
{
    bIsMoving = IsMoving;
    bIsAttacking = IsAttacking;
    bIsHit = IsHit;
    bIsDead = IsDead;
}
