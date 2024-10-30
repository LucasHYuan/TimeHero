// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUnit.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "PlayerAnimInstance.h"

APlayerUnit::APlayerUnit()
{
    Speed = 1.0f;
    AttackPower = 20.0f;
    DefensePower = 10.0f;
    RemainingTime = 100.0f;

    bIsMoving = false;
    bIsAttacking = false;
    bIsHit = false;
    bIsDead = false;
}

void APlayerUnit::BeginPlay()
{
    Super::BeginPlay();
    
    PlayerAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
    
    
}

void APlayerUnit::MoveToTile(FIntPoint Destination)
{
    if (bIsDead || bIsAttacking || bIsHit)
    {
        return;
    }
    bIsMoving = true;
    UpdateAnimationState();
}

void APlayerUnit::Attack(ABaseUnit* TargetUnit)
{

    if (bIsDead || bIsHit)
    {
        return;
    }

    bIsAttacking = true;
    UpdateAnimationState();
}

void APlayerUnit::TakeDamage(float DamageAmount)
{
    if (bIsDead)
    {
        return;
    }

    RemainingTime -= DamageAmount;
    bIsHit = true;
    UpdateAnimationState();

    if (RemainingTime <= 0)
    {
        bIsDead = true;
        UpdateAnimationState();
    }
}


void APlayerUnit::UpdateAnimationState()
{
    if (PlayerAnimInstance)
    {
        PlayerAnimInstance->UpdateAnimationStates(bIsMoving, bIsAttacking, bIsHit, bIsDead);
    }
}
