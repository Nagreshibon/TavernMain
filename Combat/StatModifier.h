// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CombatComponent.h"
#include "CoreMinimal.h"
//#include "StatModifier.generated.h"

/**
 * 
 */

struct FStat; 
class UCombatComponent; 

class TAV_API UStatModifier
{
	//GENERATED_BODY()

public:
	
		UStatModifier(FStat& inStat, float inAmount, float inDuration, UCombatComponent& inOwningComp);
		~UStatModifier();
				
		FStat& ManagedStat;
		float Amount;
		float Duration;
		UCombatComponent& OwningComp;
	
		float Ticker;
	
		void Tick(float inDelta);
	
};
