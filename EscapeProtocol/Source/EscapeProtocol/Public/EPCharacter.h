// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EPInventoryComponent.h"
#include "EPCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UEPWeaponComponent;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Unarmed		UMETA(DisplayName = "Unarmed"),
	Pistol		UMETA(DisplayName = "Pistol"),
	Rifle		UMETA(DisplayName = "Rifle"),
	Shotgun		UMETA(DisplayName = "Shotgun")
};

UCLASS()
class ESCAPEPROTOCOL_API AEPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEPCharacter();
	// 카메라 설정 관련
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    USpringArmComponent* SpringArmComp;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    UCameraComponent* CameraComp;

    // 캐릭터 무브먼트 관련
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    UCharacterMovementComponent* TPSMovementComp = GetCharacterMovement();


    // 캐릭터 Action 관련 
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartJump(const FInputActionValue& Value);
    void StopJump(const FInputActionValue& Value);
    void StartCrouch(const FInputActionValue& Value);
    void StopCrouch(const FInputActionValue& Value);
    void StartSprint(const FInputActionValue& Value);
    void StopSprint(const FInputActionValue& Value);
    void Fire(const FInputActionValue& Value);
    void Reload(const FInputActionValue& Value);
	void EquipRifle(const FInputActionValue& Value);
	void EquipShotgun(const FInputActionValue& Value);
	void EquipPistol(const FInputActionValue& Value);
	void UnEquip(const FInputActionValue& Value);
	void AimingDownSight(const FInputActionValue& Value);
	void ReleaseAimingDownSight(const FInputActionValue& Value);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float NormalGroundSpeed = 600.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintGroundSpeed = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Crouch")
    bool bIsCrouching = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Jump")
    bool bIsJumping = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Sprint")
    bool bIsSprinting = false;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "ActionState|Ads")
	void ZoomIn();
	UFUNCTION(BlueprintImplementableEvent, Category = "ActionState|Ads")
	void ZoomOut();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionState|Ads")
	bool bIsZooming = false;

	// 애니메이션 재생 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterState")
	ECharacterState CharacterState = ECharacterState::Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
	UAnimMontage* PistolFireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
	UAnimMontage* RifleFireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
	UAnimMontage* ShotgunFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
	UAnimMontage* PistolReloadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
	UAnimMontage* RifleReloadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
	UAnimMontage* ShotgunReloadMontage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
    UAnimMontage* DeathMontage;
	

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information|Inventory")
    UEPInventoryComponent* InventoryComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information|HP")
    float Health;

    UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Information|Weapon")
    UEPWeaponComponent* WeaponComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Information|Weapon")
    USkeletalMeshComponent* WeaponMeshComponent;

    USkeletalMesh* RifleMesh;

    USkeletalMesh* ShotGunMesh;

    USkeletalMesh* HandGunMesh;

    

protected:
    virtual void BeginPlay() override;

    bool bIsDead;
    

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void AddHealth(float value);

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Information|State")
    bool IsDead() const;
};