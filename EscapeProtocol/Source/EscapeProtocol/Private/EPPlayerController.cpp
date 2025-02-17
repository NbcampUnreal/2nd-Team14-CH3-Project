// Fill out your copyright notice in the Description page of Project Settings.


#include "EPPlayerController.h"
#include "EnhancedInputSubsystems.h"

AEPPlayerController::AEPPlayerController()
{
}

void AEPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AddInputMappingContext();
}

void AEPPlayerController::AddInputMappingContext() const
{
	// Local Player ��������
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	// Local Player �� Subsystem ��������
	auto* EnhancedInputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!EnhancedInputSubsystem)
	{
		return;
	}

	// Local Player �� Subsystem �� InputMappingContext �߰�
	EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0);

}
