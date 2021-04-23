// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTCleanFloor.generated.h"

/**
 * 
 */
UCLASS()
class TAV_API UBTCleanFloor : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY()
		FTimerHandle UnusedHandle;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void CleanFloorRelease(UBehaviorTreeComponent& OwnerComp);
	
};
