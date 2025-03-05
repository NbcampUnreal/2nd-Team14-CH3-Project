// Fill out your copyright notice in the Description page of Project Settings.
#include "EPWeaponHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UEPWeaponHUD::UpdateWeaponIcon(EWeaponType WeaponType)
{
    if (!PistolImage || !RifleImage || !ShotgunHUD) return;  // 🔹 ShotgunImage → ShotgunHUD

    PistolImage->SetVisibility(ESlateVisibility::Hidden);
    RifleImage->SetVisibility(ESlateVisibility::Hidden);
    ShotgunHUD->SetVisibility(ESlateVisibility::Hidden);  // 🔹 ShotgunImage → ShotgunHUD

    switch (WeaponType)
    {
    case EWeaponType::HandGun:
        PistolImage->SetVisibility(ESlateVisibility::Visible);
        break;
    case EWeaponType::Rifle:
        RifleImage->SetVisibility(ESlateVisibility::Visible);
        break;
    case EWeaponType::Shotgun:
        ShotgunHUD->SetVisibility(ESlateVisibility::Visible);  // 🔹 ShotgunImage → ShotgunHUD
        break;
    }
}

void UEPWeaponHUD::UpdateAmmo(int32 Clip, int32 MaxAmmo)
{
    if (!BulletText) return;

    FString AmmoString = FString::Printf(TEXT("%d / %d"), Clip, MaxAmmo);
    BulletText->SetText(FText::FromString(AmmoString));
}

void UEPWeaponHUD::ShowReloading(bool bIsReloading)
{
    if (!ReloadText) return;

    ReloadText->SetVisibility(bIsReloading ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
