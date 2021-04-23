// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TAV/Props/PropBar.h"
#include "BTOrderDrink.generated.h"

/**
 * 
 */
UCLASS()
class TAV_API UBTOrderDrink : public UBTTaskNode
{
	GENERATED_BODY()

	UBTOrderDrink(const FObjectInitializer& objectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void OrderCompleted(UBehaviorTreeComponent& OwnerComp);
	void AttachDrink(ADrink* inDrink, ANPC* inClient, UBehaviorTreeComponent& OwnerComp);

	ANPC* DrinkClient; 
	ADrink* DrinkToDestroy; 
	
	void KillDrink(ADrink* inDrink);

	UPROPERTY()
	APropBar* CurrentBar;

	UPROPERTY()
	UBarRequest* CurrentRequest; 

	void OrderFailed(UBehaviorTreeComponent& OwnerComp);

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	float ExpirationTimer;
	float ExpirationTick = 0; 

	UPROPERTY()
		FTimerHandle UnusedHandle;
	
};
