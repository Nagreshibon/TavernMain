// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTBartend.generated.h"

/**
 * 
 */
UCLASS()
class TAV_API UBTBartend : public UBTTaskNode
{

	GENERATED_BODY()
		
		UBTBartend();

		EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 
	
};
