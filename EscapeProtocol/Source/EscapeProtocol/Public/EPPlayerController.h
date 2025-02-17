// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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

protected:
	virtual void BeginPlay() override;

	void AddInputMappingContext() const;

};
