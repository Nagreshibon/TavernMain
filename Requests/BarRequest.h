// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TAV/Props/Drink.h"
//#include "TAV/NPC.h"
//#include "TAV/Props/PropBar.h"
#include "UObject/NoExportTypes.h"
#include "BarRequest.generated.h"

class APropBar; 
class ANPC;

UENUM(BlueprintType)
enum class ERequestPriority : uint8
{
	MAXIMUM = 0    UMETA(DisplayName = "Maximum"),
	HIGH    UMETA(DisplayName = "High"),
	NORMAL UMETA(DisplayName = "Normal"),
	LOW UMETA(DisplayName = "Normal")
};
/**
 * 
 */
UCLASS()
class TAV_API UBarRequest : public UObject
{
	GENERATED_BODY()

public:
		UBarRequest();

		UBarRequest(TSubclassOf<ADrink> inDrink, ANPC* inClient, float inExpTime, ERequestPriority inPriority);

		~UBarRequest(); 
	
	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<ADrink> DrinkClass;
	UPROPERTY(BlueprintReadWrite)
		ADrink* DrinkActual; 
	UPROPERTY(BlueprintReadWrite)
		ANPC* Client;
	UPROPERTY(BlueprintReadWrite)
		ANPC* Bartender;
	UPROPERTY(BlueprintReadWrite)
		APropBar* Bar;
	UPROPERTY(BlueprintReadWrite)
		float ExpirationTime;
	UPROPERTY(BlueprintReadWrite)
		ERequestPriority Priority;

	UPROPERTY(BlueprintReadWrite)
		bool bInvalidated = false; 

	FORCEINLINE bool operator== (const UBarRequest& rhs) const { return (DrinkClass == rhs.DrinkClass && Client == rhs.Client && ExpirationTime == rhs.ExpirationTime); }
	FORCEINLINE bool operator< (const UBarRequest& rhs) const { return Priority < rhs.Priority; }
	
};
