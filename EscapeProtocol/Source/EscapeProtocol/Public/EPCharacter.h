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
    void FireCompleted (const FInputActionValue& Value);
    void Reload(const FInputActionValue& Value);
	void EquipRifle(const FInputActionValue& Value);
	void EquipShotgun(const FInputActionValue& Value);
	void EquipPistol(const FInputActionValue& Value);
	void UnEquip(const FInputActionValue& Value);
	void AimingDownSight(const FInputActionValue& Value);
	void ReleaseAimingDownSight(const FInputActionValue& Value);

	// 웅크린 자세에서 점프가 가능하게 하기 위해 오버라이드
	virtual bool CanJumpInternal_Implementation() const override;

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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
    UAnimMontage* HitMontage;
	

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information|Inventory")
    UEPInventoryComponent* InventoryComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information|HP")
    float Health;

    UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Information|Component")
    UEPWeaponComponent* WeaponComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Information|Component")
    USkeletalMeshComponent* WeaponMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Information|Weapon")
    USkeletalMesh* RifleMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Information|Weapon")
    USkeletalMesh* ShotGunMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Information|Weapon")
    USkeletalMesh* HandGunMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Information|Component")
    TSubclassOf<UAnimInstance> RifleMeshAnim;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Information|Component")
    TSubclassOf<UAnimInstance> ShotGunMeshAnim;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Information|Component")
    TSubclassOf<UAnimInstance> HandGunMeshAnim;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information|Weapon")
    bool isFire;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information|Weapon")
    bool isReload;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information|Weapon")
    bool isBulletEmpty;
protected:
    virtual void BeginPlay() override;

    bool bIsDead;

    // DeathMontage 종료 후 후속 처리를 위한 함수
    UFUNCTION()
    void HandleDeathAfterMontage();

private:
    // 타이머 핸들
    FTimerHandle DeathTimerHandle;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    void AddHealth(float value);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Information|State")
    bool IsDead() const;
};
