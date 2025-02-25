// Fill out your copyright notice in the Description page of Project Settings.


#include "EPPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

AEPPlayerController::AEPPlayerController()
{
}

void AEPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AddInputMappingContext();

	if (HUDWidgetClass)
	{
		UUserWidget* HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
			if (HUDWidget)
			{
				HUDWidget->AddToViewport();
		}
	}
}

void AEPPlayerController::AddInputMappingContext() const
{
	// Local Player 가져오기
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	// Local Player 의 Subsystem 가져오기
	auto* EnhancedInputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!EnhancedInputSubsystem)
	{
		return;
	}

	// Local Player 의 Subsystem 에 InputMappingContext 추가
	EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0);

}
