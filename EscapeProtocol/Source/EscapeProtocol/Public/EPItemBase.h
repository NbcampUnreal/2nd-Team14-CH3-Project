// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPItemInterface.h"
#include "NiagaraComponent.h"

#include "EPItemBase.generated.h"

class USphereComponent;


UCLASS()
class ESCAPEPROTOCOL_API AEPItemBase : public AActor ,public IEPItemInterface
{

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEPItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Compoenet")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Compoenet")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Compoenet")
	USphereComponent* Collision;
	

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Compoenet")
	UNiagaraComponent* NiagaraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Compoenet")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Type")
	EItemType Type;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void ActivateItem(AActor* Activator) override;
	UFUNCTION()
	virtual EItemType GetItemType() const override;

	UFUNCTION()
	virtual void DestroyItem();

};
