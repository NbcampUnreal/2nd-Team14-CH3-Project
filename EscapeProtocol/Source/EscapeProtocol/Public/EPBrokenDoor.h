// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPBrokenDoor.generated.h"

UCLASS()
class ESCAPEPROTOCOL_API AEPBrokenDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEPBrokenDoor();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USceneComponent* SceneRoot;
	// Static Mesh, 에디터와 Blueprint에서 수정 가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UStaticMeshComponent* StaticMeshComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Visibility")
	bool bIsVisible;
	void SetActorVisibility(bool bVisible);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
