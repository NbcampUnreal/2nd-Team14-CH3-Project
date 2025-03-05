// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EPItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEPItemInterface : public UInterface
{
	GENERATED_BODY()
};


UENUM(BlueprintType)
enum class EItemType : uint8
{
	Potion,
	HandGun,
	Rifle,
	Shotgun,
	Snipe
};


/**
 * 
 */
class ESCAPEPROTOCOL_API IEPItemInterface
{
	GENERATED_BODY()



	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	

	UFUNCTION()
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) = 0;
	UFUNCTION()
	virtual void ActivateItem(AActor* Activator) = 0;
	UFUNCTION()
	virtual EItemType GetItemType() const = 0;




};
