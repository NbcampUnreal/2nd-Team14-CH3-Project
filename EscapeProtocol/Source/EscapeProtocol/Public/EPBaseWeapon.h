// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPWeaponInterface.h"

#include "EPBaseWeapon.generated.h"


UCLASS()
class ESCAPEPROTOCOL_API AEPBaseWeapon : public AActor , public IEPWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	// Sets default values
	AEPBaseWeapon();
	
protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Compoent")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Compoent")
	USkeletalMeshComponent* SkeletalMeshComp;

	

	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Property")
	EWeaponType WeaponType;

	
	


	virtual void FireGun() override;

	
	virtual void ReloadGun() override;

	
	virtual EWeaponType GetWeaponType() const override;


};
