// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTGetUp.generated.h"

/**
 * 
 */
UCLASS()
class TAV_API UBTGetUp : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void GetUpRelease(UBehaviorTreeComponent& OwnerComp);

	UPROPERTY()
		FTimerHandle UnusedHandle;
	
};
