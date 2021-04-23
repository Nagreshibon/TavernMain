// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Prop.h"
#include "Components/BoxComponent.h"
#include "Drink.h"
#include "TAV/NPC.h"
#include "TAV/Requests/BarRequest.h"
#include "PropBar.generated.h"


/**
 * 
 */


//USTRUCT(BlueprintType)
//struct FBarRequest
//{
//	GENERATED_BODY()
//
//	UPROPERTY(BlueprintReadWrite)
//	TSubclassOf<ADrink> DrinkClass;
//	UPROPERTY(BlueprintReadWrite)
//	ANPC* Client;
//	UPROPERTY(BlueprintReadWrite)
//	float ExpirationTime;
//	UPROPERTY(BlueprintReadWrite)
//	ERequestPriority Priority; 
//
//	FORCEINLINE bool operator== (const FBarRequest& rhs) const { return (DrinkClass == rhs.DrinkClass && Client == rhs.Client && ExpirationTime == rhs.ExpirationTime); }
//	FORCEINLINE bool operator< (const FBarRequest& rhs) const { return Priority < rhs.Priority; }
//	
//	FBarRequest(TSubclassOf<ADrink> inDrink, ANPC* inClient, float inExpTime, ERequestPriority inPriority)
//	{
//		DrinkClass = inDrink;
//		Client = inClient;
//		ExpirationTime = inExpTime;
//		Priority = inPriority; 
//	}
//
//	FBarRequest()
//	{
//		DrinkClass = nullptr;
//		Client = nullptr;
//		ExpirationTime = 0.f;
//		Priority = ERequestPriority::NORMAL; 
//	}
//};

UCLASS()
class TAV_API APropBar : public AProp
{
	GENERATED_BODY()

		APropBar();
	
public:
	UPROPERTY(BlueprintReadOnly)
	TArray<UBarRequest*> OrderCue;

	UFUNCTION(BlueprintCallable)
		ADrink* SpawnDrink(TSubclassOf<ADrink> inDrinkClass, float inStrength, float inQuality, ANPC* Client = nullptr);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* Countertop;
	
	/*UPROPERTY(BlueprintReadOnly)
		FBarRequest CurrentRequest; */
	
	UFUNCTION(BlueprintCallable)
		UBarRequest* GetTopRequest();
	
	UFUNCTION(BlueprintCallable)
		UBarRequest* AddRequest(TSubclassOf<ADrink> inDrinkClass, ANPC* inClient, float inExpTime, ERequestPriority inPriority);

	UFUNCTION(BlueprintCallable)
		bool RemoveRequest(UBarRequest* inRequest);

	UFUNCTION(BlueprintCallable)
		bool FulfillRequest(UBarRequest* inRequest, ADrink* inDrinkActor);

	UFUNCTION(BlueprintCallable)
		int GetCueSize();
	
	
};
