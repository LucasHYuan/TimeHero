// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownCameraController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

ATopDownCameraController::ATopDownCameraController()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	RootComponent = SpringArm;
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}


void ATopDownCameraController::BeginPlay()
{
	Super::BeginPlay();

	
}

#pragma region Input Process
// TODO - Grid Border Detection

void ATopDownCameraController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("CameraMoveRight", this, &ATopDownCameraController::MoveRight);
	PlayerInputComponent->BindAxis("CameraMoveForward", this, &ATopDownCameraController::MoveForward);
	PlayerInputComponent->BindAxis("CameraZoomIn", this, &ATopDownCameraController::ZoomIn);
	PlayerInputComponent->BindAxis("CameraYawRotate", this, &ATopDownCameraController::RotateYAW);

}

void ATopDownCameraController::MoveRight(float Value) {
	AddActorWorldOffset(FVector(0.0f,Value * MoveSpeed * GetWorld()->GetDeltaSeconds(), 0.0f), true);
}
void ATopDownCameraController::MoveForward(float Value) {
	AddActorWorldOffset(FVector(Value * MoveSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 0.0f), true);
	
}
void ATopDownCameraController::ZoomIn(float Value) {
	float NewArmLength = FMath::Clamp(SpringArm->TargetArmLength - Value * ZoomSpeed, MinZoom, MaxZoom);
	SpringArm->TargetArmLength = NewArmLength;
}
void ATopDownCameraController::RotateYAW(float Value) {
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += Value * RotateSpeed * GetWorld()->GetDeltaSeconds();
	SetActorRotation(NewRotation);
}
#pragma endregion
