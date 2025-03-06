// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "EPWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "EPPlayerController.h"
#include "DrawDebugHelpers.h"

AEPCharacter::AEPCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// 카메라 관련
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 350;
	SpringArmComp->SocketOffset = FVector(0.f, 40.f, 80.f);


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

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RifleMeshs(TEXT("/Game/Weapons/Rifle/Mesh/SK_Rifle.SK_Rifle"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HandGunMeshs(TEXT("/Game/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShotGunMeshs(TEXT("/Game/Weapons/Shotgun/Mesh/SKM_Shotgun.SKM_Shotgun"));
	
	if (RifleMeshs.Succeeded())
	{
		RifleMesh = RifleMeshs.Object;
		UE_LOG(LogTemp, Warning, TEXT("RifleMesh Succeed!!"));

	}
	if (HandGunMeshs.Succeeded())
	{
		HandGunMesh = HandGunMeshs.Object;
		UE_LOG(LogTemp, Warning, TEXT("HandGunMesh Succeed!!"));

	}
	if (ShotGunMeshs.Succeeded())
	{
		ShotGunMesh = ShotGunMeshs.Object;
		UE_LOG(LogTemp, Warning, TEXT("ShotGunMesh Succeed!!"));

	}

	WeaponMeshComponent->SetupAttachment(GetMesh(), FName("hand_r_ability_socket"));

	Health = 100.0f;
	bIsDead = false;
	InventoryComponent = CreateDefaultSubobject<UEPInventoryComponent>(TEXT("InventoryComponent"));
	WeaponComponent = CreateDefaultSubobject<UEPWeaponComponent>(TEXT("WeaponComponent"));
	this->Tags.Add(FName("Player"));

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
		bIsJumping = true;
		
		// Jump() 는 Crouch() 도중에 사용할 수 없어서 UnCrouch() 부터 실행
		if (bIsCrouching)
		{
			UnCrouch();
		}

		Jump();

	}
}

// 웅크린 상태에서 점프 가능하게 리턴 값에서 bIsCrouched 체크를 제외 
bool AEPCharacter::CanJumpInternal_Implementation() const
{
	Super::CanJumpInternal_Implementation();
	
	return TPSMovementComp->CanAttemptJump();
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
		const FVector CharacterUpVector = CharacterLocation + 150*GetActorUpVector();

		FHitResult RaycastHitInfo;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		const bool bRaycastHitResult = GetWorld()->LineTraceSingleByChannel(RaycastHitInfo, CharacterLocation, CharacterUpVector, ECC_Visibility, QueryParams);
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
		if (RifleMesh)
		{
			WeaponMeshComponent->SetSkeletalMesh(RifleMesh);
		}
		
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
		if (ShotGunMesh)
		{
			WeaponMeshComponent->SetSkeletalMesh(ShotGunMesh);
		}
		CharacterState = ECharacterState::Shotgun;
	}
}

void AEPCharacter::EquipPistol(const FInputActionValue& Value)
{
	const bool EquipInput = Value.Get<bool>();
	if (EquipInput)
	{
		if (HandGunMesh)
		{
			WeaponMeshComponent->SetSkeletalMesh(HandGunMesh);
		}
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

void AEPCharacter::AimingDownSight(const FInputActionValue& Value)
{
	const bool AdsInput = Value.Get<bool>();
	if (AdsInput)
	{
		// 블루 프린트에서 bIsAds 값에 따라 SpringArm 의 Length 를 변환 시킬 예정
		bIsZooming = true;
		ZoomIn();
	}
}

void AEPCharacter::ReleaseAimingDownSight(const FInputActionValue& Value)
{
	const bool AdsInput = Value.Get<bool>();
	if (!AdsInput)
	{
		bIsZooming = false;
		ZoomOut();
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
	if (WeaponComponent)
	{
		WeaponComponent->GunFire();
		//UE_LOG(LogTemp, Warning, TEXT("Fire!!"));

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

	if (WeaponComponent)
	{
		WeaponComponent->Reload();
		//UE_LOG(LogTemp, Warning, TEXT("Fire!!"));

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

	if (PlayerController->AdsAction)
	{
		EnhancedInputComponent->BindAction(
			PlayerController->AdsAction,
			ETriggerEvent::Triggered,
			this,
			&AEPCharacter::AimingDownSight);

		EnhancedInputComponent->BindAction(
			PlayerController->AdsAction,
			ETriggerEvent::Completed,
			this,
			&AEPCharacter::ReleaseAimingDownSight);
	}
}

float AEPCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead)
	{
		return 0.f;
	}
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	if (HitMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			float MontageDuration = AnimInstance->Montage_Play(HitMontage, 1.0f, EMontagePlayReturnType::Duration, 0.0f, true);
			UE_LOG(LogTemp, Warning, TEXT("Playing HitMontage, Duration: %f"), MontageDuration);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Player takeDamage : %f"), DamageAmount);
	Health -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Player took damage: %f, Remaining Health: %f"), DamageAmount, Health);

	// 체력이 0 이하라면 사망 처리
	if (Health <= 0.f)
	{
		Health = 0.0f;
		bIsDead = true;

		if (DeathMontage)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				float MontageDuration = AnimInstance->Montage_Play(DeathMontage, 1.0f);
				UE_LOG(LogTemp, Warning, TEXT("Playing DeathMontage, Duration: %f"), MontageDuration);
			}
		}
		// 캡슐 컴포넌트의 충돌 비활성화 (Ragdoll 동작에 방해되지 않도록)
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SetCanBeDamaged(false);

		// AnimInstance를 중지시키고 RagDoll 활성화
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
		// 애니메이션 모드를 물리 시뮬레이션에 맞게 변경
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

		AEPPlayerController* PlayerController = Cast<AEPPlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->UnPossess();
		}

		SetActorTickEnabled(false);

	}

	return DamageAmount;
}

void AEPCharacter::AddHealth(float value)
{
	Health += value;
	if (Health > 100.0f)
	{
		Health = 100.0f;
	}

}

bool AEPCharacter::IsDead() const
{
	return bIsDead;
}


