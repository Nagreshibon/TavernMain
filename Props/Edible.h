// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Prop.h"
#include "Edible.generated.h"

/**
 * 
 */
class AIngredient;

UCLASS()
class TAV_API AEdible : public AProp
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Edible")
		FTransform SocketTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Edible")
		float BarOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Edible")
		FRotator BarRoto;
	
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Edible")
		TArray<TSubclassOf<AIngredient>> Ingredients;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Edible")
		float Quality;
	
};
