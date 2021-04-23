// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCookCompleteOrder.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/Props/PropKitchenCounter.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"

EBTNodeResult::Type UBTCookCompleteOrder::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to put down AEdible"));
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropKitchenCounter* MyKitchenCounter = Cast<APropKitchenCounter>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyKitchenCounter->GetActorLocation());

				if (Distance < 300)
				{
					if (CurrentNPC->CurrentKitchenRequest)
					{
						if (CurrentNPC->PutDown(MyKitchenCounter))
						{
							UE_LOG(LogTemp, Warning, TEXT("Put down succeeded"));
							MyKitchenCounter->CompleteRequest(CurrentNPC->CurrentKitchenRequest); 
							return EBTNodeResult::Succeeded;
						}

						{
							UE_LOG(LogTemp, Warning, TEXT("Failed to put down"));
							CurrentNPC->CurrentKitchenRequest->bInvalidated = true;
							return EBTNodeResult::Failed;
						}
					}
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Failed to complete order."));
	return EBTNodeResult::Failed;
}


