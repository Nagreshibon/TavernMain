// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TAV/Prop.h"
#include "PropKitchenCounter.generated.h"

/**
 * 
 */
class UTableRequest;
class UBoxComponent;
class ANPC; 

UCLASS()
class TAV_API APropKitchenCounter : public AProp
{
	GENERATED_BODY()

		APropKitchenCounter(); 

public:
	UTableRequest* GetTopRequest(); 

	UFUNCTION(BlueprintCallable)
		void RemoveCompletedRequest(UTableRequest* inRequest);
	
	UPROPERTY(BlueprintReadWrite)
		TArray<UTableRequest*> RequestsCue;

	UPROPERTY(BlueprintReadWrite)
		TArray<UTableRequest*> RequestsCompleted;

	UPROPERTY(BlueprintReadWrite)
		TArray<ANPC*> ActiveCooks;

	UFUNCTION(BlueprintCallable)
		void CompleteRequest(UTableRequest* inRequest);

	UFUNCTION(BlueprintCallable)
		UTableRequest* GetTopCompletedRequest(); 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* Countertop;

	
	
};
