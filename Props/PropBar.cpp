// Fill out your copyright notice in the Description page of Project Settings.


#include "PropBar.h"

#include "Kismet/KismetMathLibrary.h"

APropBar::APropBar()
{
	Countertop = CreateDefaultSubobject<UBoxComponent>(TEXT("Countertop"));
	Countertop->SetupAttachment(StaticMesh); 
}

ADrink* APropBar::SpawnDrink(TSubclassOf<ADrink> inDrinkClass, float inStrength, float inQuality, ANPC* inClient)
{
	if (Countertop)
	{
		FVector RandomPoint; 
		if (inClient)
		{
			FVector R = inClient->GetActorLocation() - Countertop->GetComponentLocation();
			FVector RNorm = R;
			RNorm.Normalize();
			float Cos = RNorm.CosineAngle2D(Countertop->GetRightVector());
			FVector X = Countertop->GetRightVector() * (Cos * R.Size() - inDrinkClass->GetDefaultObject<ADrink>()->BarOffset); 

			RandomPoint = Countertop->GetComponentLocation() + X; 
		}
		else
		{
			RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Countertop->GetComponentLocation(), Countertop->GetScaledBoxExtent().RotateAngleAxis(Countertop->GetComponentRotation().Yaw, FVector(0, 0, 1)));
		}
		//UE_LOG(LogTemp, Warning, TEXT("Spawning drink: %s at coordinates: %s"), *inDrinkClass->GetDisplayNameText().ToString(), *RandomPoint.ToString());
		FActorSpawnParameters SpawnInfo;
		ADrink* NewDrink = GetWorld()->SpawnActor<ADrink>(inDrinkClass, RandomPoint, FRotator(0, UKismetMathLibrary::RandomFloatInRange(0, 360), 0));
		
		NewDrink->Quality = inQuality;
		NewDrink->Strength = inStrength;
		/*
		NewDrink->SocketTransform = inDrinkClass->GetDefaultObject<ADrink>()->SocketTransform;
		NewDrink->SetActorRotation(inDrinkClass->GetDefaultObject<ADrink>()->BarRoto); */
		return NewDrink;
	}
	return nullptr;
}

UBarRequest* APropBar::GetTopRequest()
{
	if (OrderCue.Num() != 0)
	{
		OrderCue.Sort();
		for (auto &o : OrderCue)
		{
			if (!o->Bartender) return o; 
		}
		
		//return OrderCue[0];
	}

	return nullptr; 
}

UBarRequest* APropBar::AddRequest(TSubclassOf<ADrink> inDrinkClass, ANPC* inClient, float inExpTime, ERequestPriority inPriority)
{
	UBarRequest* NewRequest = NewObject<UBarRequest>();
	NewRequest->DrinkClass = inDrinkClass;
	NewRequest->Client = inClient;
	NewRequest->Bar = this; 
	NewRequest->ExpirationTime = inExpTime;
	NewRequest->Priority = inPriority; 
	int32 idx = OrderCue.Add(NewRequest); 
	return NewRequest;
}

bool APropBar::RemoveRequest(UBarRequest* inRequest)
{
	if (OrderCue.Remove(inRequest) != 0) return true;

	return false; 
}

bool APropBar::FulfillRequest(UBarRequest* inRequest, ADrink* inDrinkActor)
{
	if (inDrinkActor->GetClass() == inRequest->DrinkClass)
	{
		inRequest->Client->DrinkRequestFulfilled(inDrinkActor);
		OrderCue.Remove(inRequest);

		return true;
	}
	return false; 
}

int APropBar::GetCueSize()
{
	return OrderCue.Num(); 
}

