// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalGenerator.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADecalGenerator::ADecalGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComponent; 
	
}

ADirtDecal* ADecalGenerator::SpawnDecal()
{
	if (DecalClasses.Num() > 0)
	{
		float Radius = SphereComponent->GetScaledSphereRadius();
		FVector Center = GetActorLocation();

		float Distance = UKismetMathLibrary::RandomFloatInRange(0, Radius);
		//FRotator Rotation = FRotator(0, UKismetMathLibrary::RandomFloatInRange(0, 360), 0);
		FVector RandomVector = Center + FVector(Distance, 0, 0).RotateAngleAxis(UKismetMathLibrary::RandomFloatInRange(0, 360), FVector(0, 0, 1));

		int i = UKismetMathLibrary::RandomIntegerInRange(0, DecalClasses.Num() - 1);

		ADirtDecal* Decal = GetWorld()->SpawnActor<ADirtDecal>(DecalClasses[i], RandomVector, FRotator(0, UKismetMathLibrary::RandomFloatInRange(0, 360),0));

		float ScaleRand = UKismetMathLibrary::RandomFloatInRange(0.5, 2); 
		Decal->SetActorRelativeScale3D(FVector(1, ScaleRand, ScaleRand));
		
		
		return Decal; 
	}
	
	return nullptr;
}

// Called when the game starts or when spawned
void ADecalGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADecalGenerator::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	if(bActive)
	{
		if(Interval>0)
		{
			Ticker += DeltaTime;
			if(Ticker>=Interval)
			{
				Ticker = 0;
				SpawnDecal(); 
			}
		}
	}
}

