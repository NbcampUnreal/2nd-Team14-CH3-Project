// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBaseWeapon.h"
#include "Engine/SkeletalMeshSocket.h"


AEPBaseWeapon::AEPBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(Scene);
	SkeletalMeshComp->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));

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




