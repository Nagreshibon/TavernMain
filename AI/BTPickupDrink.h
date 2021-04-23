// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTPickupDrink.generated.h"

class ANPC; 
/**
 * 
 */
UCLASS()
class TAV_API UBTPickupDrink : public UBTTaskNode
{
	GENERATED_BODY()

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	void PickupRelease(UBehaviorTreeComponent& OwnerComp);

	FTimerHandle UnusedHandle; 
	
};
