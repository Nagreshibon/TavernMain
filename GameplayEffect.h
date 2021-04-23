// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayEffect.generated.h"

/**
 * 
 */
UCLASS()
class TAV_API UGameplayEffect : public UObject
{
	GENERATED_BODY()

public:
		UFUNCTION(BlueprintCallable)
		bool ExecuteGameplayEffect(); 
	
};
