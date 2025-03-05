// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPItemSpanwRow.h"
#include "EPGameInstance.h"
#include "EPItemSpawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEPROTOCOL_API UEPItemSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEPItemSpawnComponent();

	UEPGameInstance* GameInstance;
	// Called when the game starts
	virtual void BeginPlay() override;

	FEPItemSpawnRow* GetRandomItem() const;

	UFUNCTION(BlueprintCallable)
	void SpawnItem(FVector Location);
		
};
