// Fill out your copyright notice in the Description page of Project Settings.


#include "PropStorage.h"

bool APropStorage::bIsInStorage(TSubclassOf<AIngredient> inIngredient)
{
	return Storage.Contains(inIngredient);
}

bool APropStorage::TakeFromStorage(TSubclassOf<AIngredient> inIngredient)
{
	if (auto &k = *Storage.Find(inIngredient))
	{
		if (k >= 1)
		{
			k = -1;
			return true;
		}
	}

	return false; 
}
