// Fill out your copyright notice in the Description page of Project Settings.

#include "BTLeaveOrdersAtKitchen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/Props/PropKitchenCounter.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"

EBTNodeResult::Type UBTLeaveOrdersAtKitchen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
		{
			if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
			{
				if (APropKitchenCounter* MyKitchenCounter = Cast<APropKitchenCounter>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
				{
					float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyKitchenCounter->GetActorLocation());

					{
						if (Distance < 250)
						{							
							MyKitchenCounter->RequestsCue.Append(CurrentNPC->CurrentTableRequests);
							
							for(auto &r : CurrentNPC->CurrentTableRequests)
							{
								r->Waiter = nullptr;
								r->KitchenCounter = MyKitchenCounter; 
							}

							CurrentNPC->CurrentTableRequests.Empty(); 
							
							return EBTNodeResult::Succeeded;
						}
					}
				}
			}
		}
		return EBTNodeResult::Failed;
}
