// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPDoor.generated.h"

class UBoxComponent;

UCLASS()
class ESCAPEPROTOCOL_API AEPDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEPDoor();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Component")
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Component")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Collision")
	UBoxComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorOpen|Collision")
	bool bDoorOpen;

	UFUNCTION()
	void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

};
