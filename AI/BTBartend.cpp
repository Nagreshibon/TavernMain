// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBartend.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "TAV/AICon.h"
#include "TAV/Props/PropBar.h"

UBTBartend::UBTBartend()
{
	//NodeName = "Bartend: Collect Request";
}

EBTNodeResult::Type UBTBartend::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropBar* MyBar = Cast<APropBar>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyBar->GetActorLocation());

				if (Distance < 300)
				{
					UBarRequest* Request = MyBar->GetTopRequest(); 
					if (Request)
					{
						CurrentNPC->CurrentBarRequest = Request;
						Request->Bartender = CurrentNPC; 
						//UE_LOG(LogTemp, Warning, TEXT("Order collected! Drink: %s"), *CurrentNPC->CurrentBarRequest->DrinkClass->GetDisplayNameText().ToString());

						if (CurrentNPC->CurrentBarRequest)
						{
							UE_LOG(LogTemp, Warning, TEXT("Order collected! Drink: %s"), *CurrentNPC->CurrentBarRequest->DrinkClass->GetDisplayNameText().ToString());
							AICon->BlackboardComp->SetValueAsObject("CurrentBarRequest", CurrentNPC->CurrentBarRequest);
							CurrentNPC->CurrentTarget = MyBar;

							if (UBarRequest* MyRequest = Cast<UBarRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentBarRequest")))
							{
								CurrentNPC->SayDrinkLine(MyRequest->DrinkClass, EDrinkLineType::BARTEND); 
							}
							return EBTNodeResult::Succeeded;
						}
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed; 
}
