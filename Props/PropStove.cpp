// Fill out your copyright notice in the Description page of Project Settings.


#include "PropStove.h"


#include "Kismet/KismetMathLibrary.h"
#include "TAV/NPC.h"

APropStove::APropStove()
{
	Stovetop = CreateDefaultSubobject<UBoxComponent>("Stovetop");
	Stovetop->SetupAttachment(StaticMesh); 
}

AEdible* APropStove::SpawnEdible(TSubclassOf<AEdible> inEdibleClass, float inQuality, ANPC* inClient)
{
	if (Stovetop)
	{
		FVector RandomPoint;
		RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Stovetop->GetComponentLocation(), Stovetop->GetScaledBoxExtent().RotateAngleAxis(Stovetop->GetComponentRotation().Yaw, FVector(0, 0, 1)));
		//UE_LOG(LogTemp, Warning, TEXT("Spawning drink: %s at coordinates: %s"), *inDrinkClass->GetDisplayNameText().ToString(), *RandomPoint.ToString());
		FActorSpawnParameters SpawnInfo;

		AEdible* NewEdible = GetWorld()->SpawnActor<AEdible>(inEdibleClass, RandomPoint, FRotator(0, UKismetMathLibrary::RandomFloatInRange(0, 360), 0));
		//AEdible* NewEdible = GetWorld()->SpawnActor<AEdible>(RandomPoint, FRotator(0, UKismetMathLibrary::RandomFloatInRange(0, 360), 0), SpawnInfo);
		//NewEdible->GetMesh()->SetStaticMesh(inDrinkClass->GetDefaultObject<ADrink>()->GetMesh()->GetStaticMesh());
		//NewDrink->GetMesh()->SetRelativeScale3D(inDrinkClass->GetDefaultObject<ADrink>()->GetMesh()->GetRelativeScale3D());
		//
		NewEdible->Quality = inQuality;
		//NewEdible->SocketTransform = inEdibleClass->GetDefaultObject<AEdible>()->SocketTransform;
		NewEdible->SetActorRotation(inEdibleClass->GetDefaultObject<AEdible>()->BarRoto);
		return NewEdible;
	}
	
	return nullptr; 
}
