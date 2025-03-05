// Fill out your copyright notice in the Description page of Project Settings.
#include "EPPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "EPWeaponHUD.h"  //  HUD 클래스 포함
#include "EPWeaponInterface.h"  //  EWeaponType 포함
#include "UObject/ConstructorHelpers.h"  // 🔹 경로 찾기를 위해 필요

AEPPlayerController::AEPPlayerController()
{
    // 🔹 HUDWidgetClass가 없으면 블루프린트 자동 설정 (올바른 경로 반영)
    if (!HUDWidgetClass)
    {
        static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/UI/WBP_HUD"));
        if (WidgetClass.Succeeded())
        {
            HUDWidgetClass = WidgetClass.Class;
            UE_LOG(LogTemp, Warning, TEXT(" HUDWidgetClass가 자동으로 설정되었습니다!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT(" HUD 위젯 블루프린트를 찾을 수 없습니다! 경로를 확인하세요. "));
        }
    }
}

void AEPPlayerController::BeginPlay()
{
    Super::BeginPlay();

    AddInputMappingContext();

    //  HUDWidgetClass가 설정되지 않았다면 강제로 수동 설정
    if (!HUDWidgetClass)
    {
        HUDWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/WBP_HUD.WBP_HUD_C"));
        if (HUDWidgetClass)
        {
            UE_LOG(LogTemp, Warning, TEXT(" HUDWidgetClass가 강제로 설정되었습니다!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT(" HUD 위젯을 찾을 수 없습니다. 블루프린트 경로를 다시 확인하세요! "));
        }
    }

    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UEPWeaponHUD>(this, HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT(" HUD 위젯이 정상적으로 생성되었습니다!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT(" HUD 위젯 생성 실패!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT(" HUDWidgetClass가 여전히 설정되지 않았습니다! 블루프린트 확인 필요!"));
    }
}

void AEPPlayerController::AddInputMappingContext() const
{
    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (!LocalPlayer)
    {
        return;
    }

    auto* EnhancedInputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!EnhancedInputSubsystem)
    {
        return;
    }

    EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0);
}

// 🔹 무기 변경 시 HUD만 변경하는 기능 추가
void AEPPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("SelectPistol", IE_Pressed, this, &AEPPlayerController::SelectPistol);
    InputComponent->BindAction("SelectRifle", IE_Pressed, this, &AEPPlayerController::SelectRifle);
    InputComponent->BindAction("SelectShotgun", IE_Pressed, this, &AEPPlayerController::SelectShotgun);
}

void AEPPlayerController::SelectPistol()
{
    if (HUDWidget)
    {
        HUDWidget->UpdateWeaponIcon(EWeaponType::HandGun);
        HUDWidget->UpdateAmmo(6, 12);  //  권총 탄약 UI 변경
    }
}

void AEPPlayerController::SelectRifle()
{
    if (HUDWidget)
    {
        HUDWidget->UpdateWeaponIcon(EWeaponType::Rifle);
        HUDWidget->UpdateAmmo(30, 120);  // 소총 탄약 UI 변경
    }
}

void AEPPlayerController::SelectShotgun()
{
    if (HUDWidget)
    {
        HUDWidget->UpdateWeaponIcon(EWeaponType::Shotgun);
        HUDWidget->UpdateAmmo(6, 6);  //  산탄총 탄약 UI 변경
    }
}
