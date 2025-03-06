// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EPItemSpanwRow.generated.h"

/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct FEPItemSpawnRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;


	
};
