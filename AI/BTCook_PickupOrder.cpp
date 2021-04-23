// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCook_PickupOrder.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"
#include "TAV/Props/PropKitchenCounter.h"

EBTNodeResult::Type UBTCook_PickupOrder::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropKitchenCounter* MyCounter = Cast<APropKitchenCounter>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyCounter->GetActorLocation());

				if (Distance < 300)
				{
					UTableRequest* Request = MyCounter->GetTopRequest();
					if (Request)
					{
						CurrentNPC->CurrentKitchenRequest = Request; 
						Request->Cook = CurrentNPC;
						//UE_LOG(LogTemp, Warning, TEXT("Order collected! Drink: %s"), *CurrentNPC->CurrentBarRequest->DrinkClass->GetDisplayNameText().ToString());

						if (CurrentNPC->CurrentKitchenRequest)
						{
							UE_LOG(LogTemp, Warning, TEXT("Order collected for cooking! Edible: %s"), *CurrentNPC->CurrentKitchenRequest->EdibleClass->GetDisplayNameText().ToString());
							AICon->BlackboardComp->SetValueAsObject("CurrentTableRequest", Request);
							//CurrentNPC->CurrentTarget = MyBar;

							//CurrentNPC->SayDrinkLine(MyRequest->DrinkClass, EDrinkLineType::BARTEND);
							
							return EBTNodeResult::Succeeded;
						}
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
