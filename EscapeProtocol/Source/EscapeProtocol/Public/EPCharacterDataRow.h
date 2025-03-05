// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EPCharacterDataRow.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FEPCharacterDataRow :public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HandGunAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 RifleAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ShotGunAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SnipeAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PotionAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Health;
};
