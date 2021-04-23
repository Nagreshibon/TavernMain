// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTCookAtStove.generated.h"

/**
 * 
 */
class UBTCookAtStove; 
struct FBTCookAtStoveMemory
{
	float ExpirationTimer;
	float ExpirationTick = 0;
	
	TWeakObjectPtr<UBTCookAtStove> Task;
};

UCLASS()
class TAV_API UBTCookAtStove : public UBTTaskNode
{
	GENERATED_BODY()

	UBTCookAtStove(const FObjectInitializer& objectInitializer);

	
	uint16 GetInstanceMemorySize() const;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

		void OrderCompleted(UBehaviorTreeComponent& OwnerComp);
		void OrderFailed(UBehaviorTreeComponent& OwnerComp);
	
		virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
		float ExpirationTimer;
		float ExpirationTick = 0;

		UPROPERTY()
			FTimerHandle UnusedHandle;
	
};
