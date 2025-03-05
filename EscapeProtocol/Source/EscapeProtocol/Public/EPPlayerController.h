// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPWeaponHUD.h"  //  HUD 클래스 포함
#include "EPWeaponInterface.h"  //  EWeaponType 포함
#include "EPPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class ESCAPEPROTOCOL_API AEPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEPPlayerController();

	// Input Mapping 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|MappingContext")
	UInputMappingContext* InputMappingContext = nullptr;

	// Movement IA 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Movement")
	UInputAction* MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Movement")
	UInputAction* LookAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Movement")
	UInputAction* JumpAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Movement")
	UInputAction* CrouchAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Movement")
	UInputAction* SprintAction = nullptr;

	// Battle IA 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Battle")
	UInputAction* FireAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Battle")
	UInputAction* ReloadAction = nullptr;

	// Equip 관련 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Equip")
	UInputAction* EquipRifleAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Equip")
	UInputAction* EquipShotgunAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Equip")
	UInputAction* EquipPistolAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Equip")
	UInputAction* UnEquipAction = nullptr;

	// Ads 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Ads")
	UInputAction* AdsAction = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UEPWeaponHUD> HUDWidgetClass;  //  기존 UUserWidget → UEPWeaponHUD로 변경

	UPROPERTY()
	UEPWeaponHUD* HUDWidget;  // HUD 인스턴스

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;  // 입력 처리 함수 추가

	void AddInputMappingContext() const;

	// 무기 선택 시 HUD 변경 기능 추가
	void SelectPistol();
	void SelectRifle();
	void SelectShotgun();
};
