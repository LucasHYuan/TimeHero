// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GridCursorManager.h"
#include "TopDownCameraController.generated.h"


UCLASS()
class TIMEHERO_API ATopDownCameraController : public APawn
{
	GENERATED_BODY()
	
public:	
	ATopDownCameraController();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	// Definte Components //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;

	// VARIABLES //
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotateSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZoomSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinZoom = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxZoom = 1200.0f;
private:
	// FUNCTIONS //
	void MoveRight(float Value);
	void MoveForward(float Value);
	void ZoomIn(float Value);
	void RotateYAW(float Value);
	UCursorComponent* CursorComponent;
};
