// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Prop.h"
#include "Edible.h"
#include "Drink.generated.h"

class AIngredient; 

UCLASS()
class TAV_API ADrink : public AEdible
{
	GENERATED_BODY()

public:
	/*UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drink")
		FTransform SocketTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drink")
		float BarOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drink")
		FRotator BarRoto; */
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drink")
		float Strength;

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Edible")
		TArray<TSubclassOf<AIngredient>> Ingredients;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Edible")
		float Quality;*/
	
};
