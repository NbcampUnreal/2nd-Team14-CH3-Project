// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBaseWeapon.h"

AEPBaseWeapon::AEPBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(Scene);


}



void AEPBaseWeapon::FireGun()
{

}

void AEPBaseWeapon::ReloadGun()
{
}

EWeaponType AEPBaseWeapon::GetWeaponType() const
{

	return WeaponType;
}




