// Fill out your copyright notice in the Description page of Project Settings.


#include "BTOrderMeal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"

UBTOrderMeal::UBTOrderMeal()
{
	bCreateNodeInstance = true;
	bNotifyTick = true; 
}

EBTNodeResult::Type UBTOrderMeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropTable* MyTable = Cast<APropTable>(CurrentNPC->CurrentChair->OwningTable))
			{
				float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyTable->GetActorLocation());

				{
					if (Distance < 250 && CurrentNPC->bIsSitting)
					{
						TSubclassOf<AEdible> PickedMeal = CurrentNPC->MealPicker();
						if (!CurrentNPC->Inventory->CanAfford(PickedMeal->GetDefaultObject<AEdible>()->GetPrice())) return EBTNodeResult::Failed; 
						
						CurrentRequest = MyTable->AddRequest(PickedMeal, CurrentNPC, 300, ERequestPriority::NORMAL);
						UE_LOG(LogTemp, Warning, TEXT("Making a meal order! NPC: %s, ID: %d"), *CurrentNPC->GetName(), CurrentRequest->GetUniqueID());
						AICon->BlackboardComp->SetValueAsObject("CurrentTableRequest", CurrentRequest);
						ExpirationTimer = 300;
						if (UTableRequest* Request = Cast<UTableRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentTableRequest")))
						{
							//CurrentNPC->SayDrinkLine(Request->DrinkClass, EDrinkLineType::ORDER);
						}
						return EBTNodeResult::InProgress;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Bar too far! / not sitting"));
					}					
				}
			}
		}
	}
	return EBTNodeResult::Failed; 
}

void UBTOrderMeal::OrderCompleted(UBehaviorTreeComponent& OwnerComp)
{
	UE_LOG(LogTemp, Warning, TEXT("Meal order completed successfuly!"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
}

void UBTOrderMeal::OrderFailed(UBehaviorTreeComponent& OwnerComp)
{
	UE_LOG(LogTemp, Warning, TEXT("Meal order expired"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}

void UBTOrderMeal::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//UE_LOG(LogTemp, Warning, TEXT("BTOrderDrink Tick: %f"), ExpirationTick);
	if (ExpirationTimer > ExpirationTick)
	{
		ExpirationTick += DeltaSeconds;


		if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
		{
			if (UTableRequest* Request = Cast<UTableRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentTableRequest")))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Checking order ID: %d"), Request->GetUniqueID());
				if (Request->bDelivered)
				{
					UE_LOG(LogTemp, Warning, TEXT("Meal order delivered!"));
					OrderCompleted(OwnerComp);
				}
			}
		}
	}
	else
	{
		ExpirationTimer = 0;
		ExpirationTick = 0;
		OrderFailed(OwnerComp);
	}
}
