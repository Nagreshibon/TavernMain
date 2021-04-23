// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTOrderMeal.generated.h"

/**
 * 
 */
class APropTable;
class UTableRequest; 

UCLASS()
class TAV_API UBTOrderMeal : public UBTTaskNode
{
	GENERATED_BODY()

		UBTOrderMeal();
	
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void OrderCompleted(UBehaviorTreeComponent& OwnerComp);

	UPROPERTY()
		APropTable* CurrentBar;

	UPROPERTY()
		UTableRequest* CurrentRequest;

	void OrderFailed(UBehaviorTreeComponent& OwnerComp);

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	float ExpirationTimer;
	float ExpirationTick = 0;

	UPROPERTY()
		FTimerHandle UnusedHandle;
	
};
