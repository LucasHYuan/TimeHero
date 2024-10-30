// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUnit.h"

ABaseUnit::ABaseUnit()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseUnit::MoveToTile(FIntPoint Destination) {
	UE_LOG(LogTemp, Log, TEXT("Moving to tile at location: %s"), *Destination.ToString());
}

void ABaseUnit::Attack(ABaseUnit* TargetUnit)
{
    if (TargetUnit)
    {
        float Damage = FMath::Max(AttackPower - TargetUnit->DefensePower, 1);
        TargetUnit->TakeDamage(Damage);
        UE_LOG(LogTemp, Log, TEXT("Attacking unit and causing %f damage"), Damage);
    }
}

void ABaseUnit::TakeDamage(float DamageAmount)
{
    RemainingTime -= DamageAmount;
    UE_LOG(LogTemp, Log, TEXT("Remaining Time after taking damage: %f"), RemainingTime);

    if (RemainingTime <= 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Unit has been destroyed"));
        Destroy();
    }
}