// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Prop.h"
#include "TAV/Requests/TableRequest.h"
#include "PropTable.generated.h"

/**
 * 
*/
class UBoxComponent; 
class APropChair; 

UCLASS()
class TAV_API APropTable : public AProp
{
	GENERATED_BODY()

		APropTable();
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* Tabletop; 
	
	UFUNCTION(BlueprintCallable)
		int GetNumOccupiedChairs(); 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int ChairSlots;

	UPROPERTY(BlueprintReadOnly)
		TArray<UTableRequest*> RequestArr;

	UFUNCTION(BlueprintCallable)
		TArray<UTableRequest*> GetRequests(); 

	UFUNCTION(BlueprintCallable)
		UTableRequest* AddRequest(TSubclassOf<AEdible> inEdibleClass, ANPC* inClient, float inExpTime, ERequestPriority inPriority);

	UFUNCTION(BlueprintCallable)
		bool RemoveRequest(UTableRequest* inRequest);

	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool bGotPendingOrders() { return (RequestArr.Num() > 0); }
	
	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int ChairSlots_Current;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int ChairSlots_Max;*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<APropChair*> ChairArray;

	virtual void Tick(float DeltaTime) override;
	
};
