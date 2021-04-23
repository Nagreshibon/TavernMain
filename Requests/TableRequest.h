// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BarRequest.h"
#include "TableRequest.generated.h"

class AEdible;
class ANPC;
class APropTable;
class APropKitchenCounter; 

UCLASS()
class TAV_API UTableRequest : public UObject
{
	GENERATED_BODY()

public:
	UTableRequest();

	UTableRequest(TSubclassOf<AEdible> inEdible, ANPC* inClient, float inExpTime, ERequestPriority inPriority);

	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<AEdible> EdibleClass;
	UPROPERTY(BlueprintReadWrite)
		AEdible* EdibleActual;
	UPROPERTY(BlueprintReadWrite)
		ANPC* Client;
	UPROPERTY(BlueprintReadWrite)
		ANPC* Waiter;
	UPROPERTY(BlueprintReadWrite)
		ANPC* Cook;
	UPROPERTY(BlueprintReadWrite)
		APropTable* Table;
	UPROPERTY(BlueprintReadWrite)
		APropKitchenCounter* KitchenCounter;
	UPROPERTY(BlueprintReadWrite)
		float ExpirationTime;
	UPROPERTY(BlueprintReadWrite)
		ERequestPriority Priority;

	UPROPERTY(BlueprintReadWrite)
		bool bInvalidated = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bDelivered = false;

	FORCEINLINE bool operator== (const UTableRequest& rhs) const { return (EdibleClass == rhs.EdibleClass && Client == rhs.Client && ExpirationTime == rhs.ExpirationTime); }
	FORCEINLINE bool operator< (const UTableRequest& rhs) const { return Priority < rhs.Priority; }
	
};
