// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "EPPlayerController.h"

AEPCharacter::AEPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// ī�޶� ����
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 400;
	SpringArmComp->SocketOffset = CameraSocketOffset;


	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	// �����Ʈ ����

	// ĳ���� ȸ�� ����
	SpringArmComp->bUsePawnControlRotation = false;
	CameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	TPSMovementComp->bOrientRotationToMovement = false;
	TPSMovementComp->bUseControllerDesiredRotation = false;
	TPSMovementComp->bAllowPhysicsRotationDuringAnimRootMotion = false;

	// �ɱ� Crouch ����
	TPSMovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	TPSMovementComp->bCanWalkOffLedgesWhenCrouching = true;

	// ĳ���� ���� ���� ����
	TPSMovementComp->MaxAcceleration = 2400.0f;
	TPSMovementComp->BrakingFrictionFactor = 1.0f;
	TPSMovementComp->BrakingFriction = 6.0f;
	TPSMovementComp->GroundFriction = 8.0f;
	TPSMovementComp->BrakingDecelerationWalking = 1400.0f;


}

void AEPCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveInput = Value.Get<FVector2D>();

	// ���Է� ����ó��
	if (MoveInput != FVector2D::ZeroVector)
	{
		// ĳ������ ���� ���
		const FVector DesireMoveDirection = GetActorForwardVector() * MoveInput.X + GetActorRightVector() * MoveInput.Y;

		// �밢�� �̵��� ũ�⿡ ���� ����
		const FVector CharacterXYMoveDirection = DesireMoveDirection.GetSafeNormal();


		if (TPSMovementComp->IsFalling())
		{
			AddMovementInput(CharacterXYMoveDirection, 1);
			return;
		}


		// Raycast ������
		// ���߿��� ������ ���� �� ���� ���� �ʱ� ����
		FVector FloorNormal = FVector(0.f, 0.f, 1.f);

		// Raycast �� �������� ������ ����ϱ� ���� �ʿ�
		const FVector CharacterLocation = GetActorLocation();
		const FVector FootRayDirection = CharacterLocation - FVector(0.f, 0.f, 500.f);

		FHitResult RaycastHitInfo;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		const bool bRaycastHitResult = GetWorld()->LineTraceSingleByChannel(RaycastHitInfo, CharacterLocation, FootRayDirection, ECC_Visibility, QueryParams);
		if (bRaycastHitResult)
		{
			FloorNormal = RaycastHitInfo.Normal;
		}

		const FVector AdjustedMoveDirection = FVector::VectorPlaneProject(CharacterXYMoveDirection, FloorNormal);
		const FVector CharacterMoveDirection = AdjustedMoveDirection.GetSafeNormal();

		AddMovementInput(CharacterMoveDirection, 1);
	}
}

void AEPCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();
	// ���Է� ����ó��
	if (LookInput != FVector2D::ZeroVector)
	{
		AddControllerYawInput(LookInput.X);
		AddControllerPitchInput(LookInput.Y);
	}
}

void AEPCharacter::StartJump(const FInputActionValue& Value)
{
	const bool JumpInput = Value.Get<bool>();
	if (JumpInput)
	{
		// Jump() �� Crouch() ���߿� ����� �� ��� UnCrouch() ���� ����
		if (bIsCrouching)
		{
			UnCrouch();
		}

		Jump();
		bIsJumping = true;

	}
}

void AEPCharacter::StopJump(const FInputActionValue& Value)
{
	const bool JumpInput = Value.Get<bool>();
	// ���Է� ����ó��
	if (!JumpInput)
	{
		StopJumping();
		bIsJumping = false;
	}
}

void AEPCharacter::StartCrouch(const FInputActionValue& Value)
{
	const bool CrouchInput = Value.Get<bool>();
	// ���Է� ����ó��
	if (CrouchInput && !TPSMovementComp->IsFalling() && !bIsJumping)
	{
		Crouch();
		bIsCrouching = true;

	}
}

void AEPCharacter::StopCrouch(const FInputActionValue& Value)
{
	const bool CrouchInput = Value.Get<bool>();
	// ���Է� ����ó��
	if (!CrouchInput)
	{
		UnCrouch();
		bIsCrouching = false;

	}
}

void AEPCharacter::StartSprint(const FInputActionValue& Value)
{
	const bool SprintInput = Value.Get<bool>();
	// ���Է� ����ó��
	if (SprintInput)
	{
		TPSMovementComp->MaxWalkSpeed = SprintGroundSpeed;

	}
}

void AEPCharacter::StopSprint(const FInputActionValue& Value)
{
	const bool SprintInput = Value.Get<bool>();
	// ���Է� ����ó��
	if (!SprintInput)
	{
		TPSMovementComp->MaxWalkSpeed = NormalGroundSpeed;

	}
}

void AEPCharacter::Fire(const FInputActionValue& Value)
{
}

void AEPCharacter::Reload(const FInputActionValue& Value)
{
}

void AEPCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AEPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// BindAction �� ���� EnhancedInputComponent ��������
	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		return;
	}

	// BindAction �� ���� PlayerController ��������
	const auto* PlayerController = Cast<AEPPlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}

	// Controller �� ������ IA �� ��ϵǾ� �ִ��� Ȯ��

	if (PlayerController->MoveAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::Move);
	}

	if (PlayerController->LookAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->LookAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::Look);
	}

	if (PlayerController->JumpAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->JumpAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::StartJump);

		EnhancedInputComponent->BindAction(
			PlayerController->JumpAction,
			ETriggerEvent::Completed,
			this,
			&AEPCharacter::StopJump);
	}

	if (PlayerController->CrouchAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->CrouchAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::StartCrouch);

		EnhancedInputComponent->BindAction(
			PlayerController->CrouchAction,
			ETriggerEvent::Completed,
			this,
			&AEPCharacter::StopCrouch);
	}

	if (PlayerController->SprintAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->SprintAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::StartSprint);

		EnhancedInputComponent->BindAction(
			PlayerController->SprintAction,
			ETriggerEvent::Completed,
			this,
			&AEPCharacter::StopSprint);
	}

	if (PlayerController->FireAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->FireAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::Fire);
	}

	if (PlayerController->ReloadAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->ReloadAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::Reload);
	}
}

