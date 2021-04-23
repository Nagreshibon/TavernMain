// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TAV/Props/PropBar.h"
#include "BTMakeDrink.generated.h"

/**
 * 
 */
UCLASS()
class TAV_API UBTMakeDrink : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void DrinkCompleted(UBehaviorTreeComponent& OwnerComp);

	APropBar* CurrentBar;
	UBarRequest* CurrentRequest;

	void DrinkFailed(UBehaviorTreeComponent& OwnerComp);

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	float ExpirationTimer;
	float ExpirationTick = 0;

	UPROPERTY()
		FTimerHandle UnusedHandle;
	
};
