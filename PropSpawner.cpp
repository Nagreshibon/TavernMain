// Fill out your copyright notice in the Description page of Project Settings.


#include "PropSpawner.h"
#include "Components/DecalComponent.h"
#include "Prop.h"

// Sets default values
APropSpawner::APropSpawner()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CollisionCapsule->SetupAttachment(StaticMesh);
	CollisionCapsule->SetCapsuleRadius(150);
	CollisionCapsule->SetCapsuleHalfHeight(100);

	CollisionCapsule->SetGenerateOverlapEvents(true);
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &APropSpawner::SetConstructionInvalid);
	CollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &APropSpawner::SetConstructionValid);

	ArrowDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("ArrowDecal"));
	ArrowDecal->SetupAttachment(RootComponent);

	RotationTarget = nullptr; 
	
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APropSpawner::SetConstructionInvalid(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentPropClass)
	{
		if (CurrentPropClass->GetDefaultObject<AProp>()->PostOverlap(Comp, OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlap detected! Invalidating construction"));
			bConstructionValid = false;
			StaticMesh->SetMaterial(0, Mat_Invalid);
		}
	}
}

void APropSpawner::SetConstructionValid(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CurrentPropClass)
	{
		if (CurrentPropClass->GetDefaultObject<AProp>()->PostEndOverlap(Comp, OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlap ended! Validating construction"));
			bConstructionValid = true;
			StaticMesh->SetMaterial(0, Mat_Valid);
		}
	}
}

void APropSpawner::SetConstructionValid()
{
	UE_LOG(LogTemp, Warning, TEXT("Validating construction (blind)"));
	bConstructionValid = true;
	StaticMesh->SetMaterial(0, Mat_Valid);
}

void APropSpawner::SetConstructionInvalid()
{
	UE_LOG(LogTemp, Warning, TEXT("Invalidating construction (blind)"));
	bConstructionValid = false;
	StaticMesh->SetMaterial(0, Mat_Invalid);
}

// Called when the game starts or when spawned
void APropSpawner::BeginPlay()
{
	Super::BeginPlay(); 
	
	SetActorHiddenInGame(true);

	CollisionCapsule->SetGenerateOverlapEvents(true);
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &APropSpawner::SetConstructionInvalid);
	CollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &APropSpawner::SetConstructionValid);
	
}

void APropSpawner::SetProp(TSubclassOf<class AProp> inPropClass)
{
	if (AProp* DefaultObject = inPropClass->GetDefaultObject<AProp>())
	{
		RotationTarget = nullptr; 
		CurrentPropClass = inPropClass;
		CurrentPropClass = DefaultObject->GetClass();
		StaticMesh->SetStaticMesh(DefaultObject->GetMesh()->GetStaticMesh());
		if (Mat_Valid && Mat_Invalid)
		{
			StaticMesh->SetMaterial(0, Mat_Valid);
		}

		CollisionCapsule->SetGenerateOverlapEvents(true);
		CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &APropSpawner::SetConstructionInvalid);
		CollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &APropSpawner::SetConstructionValid);

		CollisionCapsule->SetCapsuleRadius(DefaultObject->CollisionCapsule->GetScaledCapsuleRadius());
		CollisionCapsule->SetCapsuleRadius(DefaultObject->CollisionCapsule->GetScaledCapsuleHalfHeight());
		CollisionCapsule->SetRelativeLocationAndRotation(DefaultObject->CollisionCapsule->GetRelativeLocation(), DefaultObject->CollisionCapsule->GetRelativeRotation());

		StaticMesh->SetRelativeScale3D(DefaultObject->GetMesh()->GetRelativeScale3D());
		StaticMesh->SetRelativeLocationAndRotation(DefaultObject->GetMesh()->GetRelativeLocation(), DefaultObject->GetMesh()->GetRelativeRotation());
		
		if(DefaultObject->ArrowDecal)
		{
			ArrowDecal->SetMaterial(0, DefaultObject->ArrowDecal->GetMaterial(0));
			ArrowDecal->SetRelativeScale3D(DefaultObject->ArrowDecal->GetRelativeScale3D());
			ArrowDecal->SetRelativeLocationAndRotation(DefaultObject->ArrowDecal->GetRelativeLocation(), DefaultObject->ArrowDecal->GetRelativeRotation());
		}

		if (DefaultObject->bStartInvalid) SetConstructionInvalid();
		else SetConstructionValid(); 
	}
}

// Called every frame
void APropSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(RotationTarget)
	{
		FRotator Rotation = (RotationTarget->GetActorLocation() - GetActorLocation()).ToOrientationRotator();

		SetActorRotation(Rotation);
	}
}

