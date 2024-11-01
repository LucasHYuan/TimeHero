// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownCameraController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

ATopDownCameraController::ATopDownCameraController()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	RootComponent = SpringArm;
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	CursorComponent = CreateDefaultSubobject<UCursorComponent>("CursorComponent");
}


void ATopDownCameraController::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "Cursor");
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
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
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, CursorComponent, &UCursorComponent::OnLeftMouseClick);
	PlayerInputComponent->BindAction("RightMouseClick", IE_Pressed, CursorComponent, &UCursorComponent::OnRightMouseClick);
}

void ATopDownCameraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, "Camear::Tick");
	CursorComponent->HandleTick(DeltaTime);
}

void ATopDownCameraController::MoveRight(float Value) {
	FVector RightDirection = GetActorRightVector();
	RightDirection.Z = 0.0f;
	RightDirection.Normalize();

	AddActorWorldOffset(RightDirection * Value * MoveSpeed * GetWorld()->GetDeltaSeconds(), true);
}

void ATopDownCameraController::MoveForward(float Value) {
	FVector ForwardDirection = GetActorForwardVector();
	ForwardDirection.Z = 0.0f;
	ForwardDirection.Normalize();

	AddActorWorldOffset(ForwardDirection * Value * MoveSpeed * GetWorld()->GetDeltaSeconds(), true);
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
