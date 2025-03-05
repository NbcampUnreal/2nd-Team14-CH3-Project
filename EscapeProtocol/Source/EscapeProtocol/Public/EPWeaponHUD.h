// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPWeaponInterface.h"  // ?? 기존 EWeaponType 정의된 파일 포함
#include "EPWeaponHUD.generated.h"

UCLASS()
class ESCAPEPROTOCOL_API UEPWeaponHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    class UImage* PistolImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* RifleImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* ShotgunHUD;  // ?? ShotgunImage → ShotgunHUD (예약어 충돌 방지)

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BulletText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ReloadText;

    void UpdateWeaponIcon(EWeaponType WeaponType);
    void UpdateAmmo(int32 Clip, int32 MaxAmmo);
    void ShowReloading(bool bIsReloading);
};
