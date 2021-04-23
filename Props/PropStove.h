// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "TAV/Prop.h"
#include "PropStove.generated.h"

/**
 * 
 */
class ANPC; 
class AEdible;
class AIngredient; 

UCLASS()
class TAV_API APropStove : public AProp
{
	GENERATED_BODY()

		APropStove(); 

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* Stovetop;

	UFUNCTION()
		AEdible* SpawnEdible(TSubclassOf<AEdible> inEdibleClass, float inQuality, ANPC* inClient);

};
