// Fill out your copyright notice in the Description page of Project Settings.


#include "BTWaiter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"

EBTNodeResult::Type UBTWaiter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropTable* MyTable = Cast<APropTable>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyTable->GetActorLocation());

				if (Distance < 300)
				{
					TArray<UTableRequest*> Requests = MyTable->GetRequests();
					if (Requests.Num() > 0)
					{
						CurrentNPC->CurrentTableRequests.Append(Requests); 
						for (auto& r : Requests)
						{
							//CurrentNPC->CurrentBarRequest = Request;
							r->Waiter = CurrentNPC;
							UE_LOG(LogTemp, Warning, TEXT("Order collected! Edible: %s"), *r->EdibleClass->GetDisplayNameText().ToString());
							MyTable->RequestArr.Remove(r); 
							//if (CurrentNPC->CurrentBarRequest)
							//{
							//	//UE_LOG(LogTemp, Warning, TEXT("Order collected! Drink: %s"), *CurrentNPC->CurrentBarRequest->DrinkClass->GetDisplayNameText().ToString());
							//	//AICon->BlackboardComp->SetValueAsObject("CurrentBarRequest", CurrentNPC->CurrentBarRequest);
							//	CurrentNPC->CurrentTarget = MyBar;

							//	if (UBarRequest* MyRequest = Cast<UBarRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentBarRequest")))
							//	{
							//		CurrentNPC->SayDrinkLine(MyRequest->DrinkClass, EDrinkLineType::BARTEND);
							//	}
							//	
							//}
						}
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;
	
}
