// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EPItemBase.h"
#include "EPBulletItem.generated.h"

/**
 * 
 */


UCLASS()
class ESCAPEPROTOCOL_API AEPBulletItem : public AEPItemBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AEPBulletItem();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	int32 Amount;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void ActivateItem(AActor* Activator) override;

private:

	int32 GetBulletAmount();


};
