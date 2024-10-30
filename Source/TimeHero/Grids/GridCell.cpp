// Fill out your copyright notice in the Description page of Project Settings.


#include "GridCell.h"
#include "Components/InstancedStaticMeshComponent.h"

AGridCell::AGridCell()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGridCell::SetInstancedStaticMeshComponent(UInstancedStaticMeshComponent* IMC)
{
	InstancedMeshComponent = IMC;
}

void AGridCell::SetPos(int32 x, int32 y) {
	X = x;
	Y = y;
}

//TODO - Abstract A Generate Method, Pass a Path Text, called by a generator
void AGridCell::GeneratePlayer() {
	UClass* PlayerUnitClass = LoadClass<APlayerUnit>(nullptr, TEXT("/Game/Blueprints/Units/BP_PlayerUnit.BP_PlayerUnit_C"));

	if (PlayerUnitClass)
	{
		FTransform InstanceTransform;
		if (InstancedMeshComponent->GetInstanceTransform(MeshIdx, InstanceTransform, true))
		{
			FVector SpawnLocation = InstanceTransform.GetLocation();
			FRotator SpawnRotation = InstanceTransform.GetRotation().Rotator();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			Unit = GetWorld()->SpawnActor<APlayerUnit>(PlayerUnitClass, SpawnLocation, SpawnRotation, SpawnParams);

			if (Unit)
			{
				Unit->Speed = 1.5f;
				Unit->AttackPower = 30.0f;
			}
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "No such class");
	}
}

void AGridCell::Selected()
{
	if (bIsClicked)return;
	if (MeshIdx == -1)return;
	InstancedMeshComponent->SetCustomDataValue(MeshIdx, 0, 1);
}

void AGridCell::UnSelected()
{
	if (bIsClicked)return;
	if (MeshIdx == -1)return;
	InstancedMeshComponent->SetCustomDataValue(MeshIdx, 0, 0);
}

void AGridCell::Clicked()
{
	bIsClicked = true;
	if (MeshIdx == -1)return;
	InstancedMeshComponent->SetCustomDataValue(MeshIdx, 0, 1);
	InstancedMeshComponent->SetCustomDataValue(MeshIdx, 1, 1);
	GeneratePlayer();
}

void AGridCell::OnPath()
{
	if (bIsClicked)return;
	if (MeshIdx == -1)return;
	InstancedMeshComponent->SetCustomDataValue(MeshIdx, 0, 1);
	InstancedMeshComponent->SetCustomDataValue(MeshIdx, 1, 1);
	InstancedMeshComponent->SetCustomDataValue(MeshIdx, 2, 1);
}

void AGridCell::BeginPlay()
{
	Super::BeginPlay();
}


