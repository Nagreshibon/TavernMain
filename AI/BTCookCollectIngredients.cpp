// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCookCollectIngredients.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/Props/PropKitchenCounter.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"

EBTNodeResult::Type UBTCookCollectIngredients::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropStorage* MyStorage = Cast<APropStorage>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyStorage->GetActorLocation());

				if (Distance < 300)
				{
					if (CurrentNPC->CurrentKitchenRequest)
					{
						if (CurrentNPC->CollectIngredients(CurrentNPC->CurrentKitchenRequest, MyStorage).Num() > 0)
						{							
							UE_LOG(LogTemp, Warning, TEXT("Ingredients collected successfully!"));
							//AICon->BlackboardComp->SetValueAsObject("CurrentTarget", CurrentNPC->CurrentKitchenRequest->KitchenCounter);
							return EBTNodeResult::Succeeded;
						}

						{
							UE_LOG(LogTemp, Warning, TEXT("No ingredients!"));
							CurrentNPC->CurrentKitchenRequest->bInvalidated = true;
							return EBTNodeResult::Failed;
						}
					}
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Failed to collect ingredients."));
	return EBTNodeResult::Failed;
}
