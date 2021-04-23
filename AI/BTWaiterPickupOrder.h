// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTWaiterPickupOrder.generated.h"

/**
 * 
 */
UCLASS()
class TAV_API UBTWaiterPickupOrder : public UBTTaskNode
{
	GENERATED_BODY()

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	void PickupRelease(UBehaviorTreeComponent& OwnerComp);

	FTimerHandle UnusedHandle; 
	
};
