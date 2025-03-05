// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EPItemBase.h"
#include "EPItemPotion.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEPROTOCOL_API AEPItemPotion : public AEPItemBase
{
	GENERATED_BODY()


public:
	AEPItemPotion();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	float HealValue;
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ActivateItem(AActor* Activator) override;

};
