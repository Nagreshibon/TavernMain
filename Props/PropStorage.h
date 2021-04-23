// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Prop.h"
#include "TAV/Ingredient.h"
#include "PropStorage.generated.h"


/**
 * 
 */
UCLASS()
class TAV_API APropStorage : public AProp
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AIngredient>, unsigned int> Storage;

	bool bIsInStorage(TSubclassOf<AIngredient> inIngredient);

	bool TakeFromStorage(TSubclassOf<AIngredient> inIngredient); 
	
};
