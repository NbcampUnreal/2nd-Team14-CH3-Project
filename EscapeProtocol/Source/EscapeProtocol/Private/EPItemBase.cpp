// Fill out your copyright notice in the Description page of Project Settings.


#include "EPItemBase.h"
#include"Components/SphereComponent.h"

// Sets default values
AEPItemBase::AEPItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Scene);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	FTransform CollisionTransForm;
	CollisionTransForm.SetLocation(FVector(0.0f, 0.0f, 70.0f));
	Collision->SetWorldTransform(CollisionTransForm);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	StaticMesh->SetWorldRotation(FRotator(40.0f, 0.0f, 0.0f));
	StaticMesh->SetWorldScale3D(FVector(0.5f));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AEPItemBase::OnItemOverlap);

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComp->SetupAttachment(Scene);
	Type = EItemType::HandGun;
	RotationSpeed = 50.0f;
}

void AEPItemBase::BeginPlay()
{
	Super::BeginPlay();

}


void AEPItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
}

void AEPItemBase::OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!")));
		ActivateItem(OtherActor);
	}
}


void AEPItemBase::ActivateItem(AActor* Activator)
{
}

EItemType AEPItemBase::GetItemType() const
{
	return Type;
}



void AEPItemBase::DestroyItem()
{
	NiagaraComp->Deactivate();
	Destroy();
}

