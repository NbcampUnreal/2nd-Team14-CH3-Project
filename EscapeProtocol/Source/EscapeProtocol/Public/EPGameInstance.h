// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEPROTOCOL_API UEPGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UEPGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EPGameData")
	int32 TotalKilledEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EPGameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToTotalKilledEnemy(int32 Amount);
};
