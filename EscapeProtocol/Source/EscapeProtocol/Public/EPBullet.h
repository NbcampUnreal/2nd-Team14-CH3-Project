// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPBullet.generated.h"

UCLASS()
class ESCAPEPROTOCOL_API AEPBullet : public AActor
{
	GENERATED_BODY()

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USceneComponent* Senen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float Range;

	// Sets default values for this actor's properties
	AEPBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
