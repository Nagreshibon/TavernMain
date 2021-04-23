// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop.h"
#include "Components/DecalComponent.h"

// Sets default values
AProp::AProp()
{

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot; 
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CollisionCapsule->SetupAttachment(StaticMesh);
	CollisionCapsule->SetCapsuleRadius(150);
	CollisionCapsule->SetCapsuleHalfHeight(100);

	ArrowDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("ArrowDecal"));
	ArrowDecal->SetupAttachment(RootComponent);
	ArrowDecal->SetHiddenInGame(true);
	ArrowDecal->SetVisibility(false); 

	bStartInvalid = false; 
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AProp::Construct(float inAmount)
{
	ConstructCurrent += inAmount;

	ConstructionMaterialInstance->SetScalarParameterValue(FName("ExponentIn"), OriginalMaterialExponent - (OriginalMaterialExponent * (ConstructCurrent / ConstructTotal)));
	
	if(ConstructCurrent>=ConstructTotal)
	{
		StaticMesh->SetMaterial(0, OriginalMaterial); 
	}
}

bool AProp::PostOverlap(UPrimitiveComponent* Comp, AActor* OtherActor)
{
	return true; 
}

bool AProp::PostEndOverlap(UPrimitiveComponent* Comp, AActor* OtherActor)
{
	return true;
}

// Called when the game starts or when spawned
void AProp::BeginPlay()
{
	Super::BeginPlay();

	Anchor = GetActorTransform();
	if (ConstructTotal > ConstructCurrent)
	{
		if (UnderConstructionMaterial)
		{
			ConstructionMaterialInstance = UMaterialInstanceDynamic::Create(StaticMesh->GetMaterial(0), this);
			FHashedMaterialParameterInfo MatInfo(TEXT("ExponentIn")); 
			ConstructionMaterialInstance->GetScalarParameterValue(MatInfo, OriginalMaterialExponent, false);
			OriginalMaterial = StaticMesh->GetMaterial(0); 
			StaticMesh->SetMaterial(0, UnderConstructionMaterial);
		}
	}
	
}

// Called every frame
void AProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

