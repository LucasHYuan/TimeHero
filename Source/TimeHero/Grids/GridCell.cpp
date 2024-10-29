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


