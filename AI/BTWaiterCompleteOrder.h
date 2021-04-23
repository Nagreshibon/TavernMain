// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTWaiterCompleteOrder.generated.h"

/**
 * 
 */
UCLASS()
class TAV_API UBTWaiterCompleteOrder : public UBTTaskNode
{
	GENERATED_BODY()

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	void PutdownRelease(UBehaviorTreeComponent& OwnerComp);

	FTimerHandle UnusedHandle;
	
};
