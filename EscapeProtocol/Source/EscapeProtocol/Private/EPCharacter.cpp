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
	PrimaryActorTick.bCanEverTick = false;

	// 카메라 관련
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 350;
	SpringArmComp->SocketOffset = CameraSocketOffset;


	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	// 무브먼트 관련
	TPSMovementComp->MaxWalkSpeed = 400.0f;

	// 캐릭터 회전 관련
	SpringArmComp->bUsePawnControlRotation = true;
	CameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	TPSMovementComp->bOrientRotationToMovement = false;
	TPSMovementComp->bUseControllerDesiredRotation = false;
	TPSMovementComp->bAllowPhysicsRotationDuringAnimRootMotion = false;

	// 앉기 Crouch 관련
	TPSMovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	TPSMovementComp->bCanWalkOffLedgesWhenCrouching = true;
	TPSMovementComp->SetCrouchedHalfHeight(60.f);

	// 캐릭터 감속 관련 설정
	TPSMovementComp->MaxAcceleration = 2400.0f;
	TPSMovementComp->BrakingFrictionFactor = 1.0f;
	TPSMovementComp->BrakingFriction = 6.0f;
	TPSMovementComp->GroundFriction = 8.0f;
	TPSMovementComp->BrakingDecelerationWalking = 1400.0f;


	Health = 100.0f;
	InventoryComponent = CreateDefaultSubobject<UEPInventoryComponent>(TEXT("InventoryComponent"));

	//this->Tags.Add(FName("Player"));

}


void AEPCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveInput = Value.Get<FVector2D>();

	// 미입력 예외처리
	if (MoveInput != FVector2D::ZeroVector)
	{
		// 캐릭터의 방향 계산
		const FVector DesireMoveDirection = GetActorForwardVector() * MoveInput.X + GetActorRightVector() * MoveInput.Y;

		// 대각선 이동에 크기에 대한 보정
		const FVector CharacterXYMoveDirection = DesireMoveDirection.GetSafeNormal();


		if (TPSMovementComp->IsFalling())
		{
			AddMovementInput(CharacterXYMoveDirection, 1);
			return;
		}


		// Raycast 시작점
		// 공중에서 스폰이 시작 될 때를 위한 초기 설정
		FVector FloorNormal = FVector(0.f, 0.f, 1.f);

		// Raycast 의 시작점과 끝점을 계산하기 위해 필요
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
	// 미입력 예외처리
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
		// Jump() 는 Crouch() 도중에 사용할 수 없어서 UnCrouch() 부터 실행
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
	// 미입력 예외처리
	if (!JumpInput)
	{
		StopJumping();
		bIsJumping = false;
	}
}

void AEPCharacter::StartCrouch(const FInputActionValue& Value)
{
	const bool CrouchInput = Value.Get<bool>();
	// 미입력 예외처리
	if (CrouchInput && !TPSMovementComp->IsFalling() && !bIsJumping)
	{
		Crouch();
		bIsCrouching = true;

	}
}

void AEPCharacter::StopCrouch(const FInputActionValue& Value)
{
	const bool CrouchInput = Value.Get<bool>();
	// 미입력 예외처리
	if (!CrouchInput)
	{
		const FVector CharacterLocation = GetActorLocation();
		const FVector CharacterUpVector = GetActorUpVector();
		const FVector UpRayDirection = 100.f*CharacterUpVector;

		FHitResult RaycastHitInfo;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		const bool bRaycastHitResult = GetWorld()->LineTraceSingleByChannel(RaycastHitInfo, CharacterLocation, UpRayDirection, ECC_Visibility, QueryParams);
		if (bRaycastHitResult)
		{
			return;
		}
		
		UnCrouch();
		bIsCrouching = false;

	}

}

void AEPCharacter::StartSprint(const FInputActionValue& Value)
{
	const bool SprintInput = Value.Get<bool>();
	// 미입력 예외처리
	if (SprintInput)
	{
		TPSMovementComp->MaxWalkSpeed = SprintGroundSpeed;
		bIsSprinting = true;
	}
}

void AEPCharacter::StopSprint(const FInputActionValue& Value)
{
	const bool SprintInput = Value.Get<bool>();
	// 미입력 예외처리
	if (!SprintInput)
	{
		TPSMovementComp->MaxWalkSpeed = NormalGroundSpeed;
		bIsSprinting = false;
	}
}

void AEPCharacter::EquipRifle(const FInputActionValue& Value)
{
	const bool EquipInput = Value.Get<bool>();
	if (EquipInput)
	{
		CharacterState = ECharacterState::Rifle;
		UE_LOG(LogTemp, Display, TEXT("EquipRifle"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Fail to EquipRifle"));
	}
}

void AEPCharacter::EquipShotgun(const FInputActionValue& Value)
{
	const bool EquipInput = Value.Get<bool>();
	if (EquipInput)
	{
		CharacterState = ECharacterState::Shotgun;
	}
}

void AEPCharacter::EquipPistol(const FInputActionValue& Value)
{
	const bool EquipInput = Value.Get<bool>();
	if (EquipInput)
	{
		CharacterState = ECharacterState::Pistol;
	}
}

void AEPCharacter::UnEquip(const FInputActionValue& Value)
{
	const bool UnEquipInput = Value.Get<bool>();
	if (UnEquipInput)
	{
		CharacterState = ECharacterState::Unarmed;
	}
}


void AEPCharacter::Fire(const FInputActionValue& Value)
{
	// // 연사 사격 시
	// const FName FireSectionName = FName("Fire");

	UAnimMontage* FireMontage = nullptr;

	if (CharacterState == ECharacterState::Unarmed)
	{
		return;
	}
	
	switch (CharacterState)
	{
	case ECharacterState::Pistol :
		{
			if (PistolFireMontage)
			{
				FireMontage = PistolFireMontage;
			}
			break;	
		}
	case ECharacterState::Rifle :
		{
			if (RifleFireMontage)
			{
				FireMontage = RifleFireMontage;
			}
			break;
		}
		
	case ECharacterState::Shotgun :
		{
			if (ShotgunFireMontage)
			{
				FireMontage = ShotgunFireMontage;
			}
			break;
		}
	default :
		{
			CharacterState = ECharacterState::Unarmed;
			// 무장중이 아니라는 것을 알 수 있는 로직 (ex : 사운드, UI 출력 등)
			return;
		}
	}

	if (FireMontage)
	{
		FName FireSection = FName(TEXT("Fire"));
		PlayAnimMontage(FireMontage, 1, FireSection);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Fire Montage."))
	}
	
}

void AEPCharacter::Reload(const FInputActionValue& Value)
{
	UAnimMontage* ReloadMontage = nullptr;
	if (CharacterState == ECharacterState::Unarmed)
	{
		return;
	}
	
	switch (CharacterState)
	{
	case ECharacterState::Pistol :
		{
			if (PistolReloadMontage)
			{
				ReloadMontage = PistolReloadMontage;
			}
			break;	
		}
	case ECharacterState::Rifle :
		{
			if (RifleReloadMontage)
			{
				ReloadMontage = RifleReloadMontage;
				UE_LOG(LogTemp, Display, TEXT("Play RifleReloadMontage"));
			}
			break;
		}
		
	case ECharacterState::Shotgun :
		{
			if (ShotgunReloadMontage)
			{
				ReloadMontage = ShotgunReloadMontage;
				UE_LOG(LogTemp, Display, TEXT("Play ShotgunReloadMontage"));
			}
			break;
		}
	default :
		{
			CharacterState = ECharacterState::Unarmed;
			// 무장중이 아니라는 것을 알 수 있는 로직 (ex : 사운드, UI 출력 등)
			return;
		}
	}

	if (ReloadMontage)
	{
		FName ReloadSection = FName(TEXT("Reload"));
		PlayAnimMontage(ReloadMontage, 1, ReloadSection);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Reload Montage."))
	}
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

	// BindAction 을 위한 EnhancedInputComponent 가져오기
	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		return;
	}

	// BindAction 을 위한 PlayerController 가져오기
	const auto* PlayerController = Cast<AEPPlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}             

	// Controller 에 각각의 IA 이 등록되어 있는지 확인

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

	if (PlayerController->EquipRifleAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->EquipRifleAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::EquipRifle);
	}

	if (PlayerController->EquipShotgunAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->EquipShotgunAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::EquipShotgun);
	}

	if (PlayerController->EquipPistolAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->EquipPistolAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::EquipPistol);
	}

	if (PlayerController->UnEquipAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->UnEquipAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::UnEquip);
	}
}

void AEPCharacter::AddHealth(float value)
{
	Health += value;
	if (Health > 100.0f)
	{
		Health = 100.0f;
	}

}


