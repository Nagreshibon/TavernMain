// Fill out your copyright notice in the Description page of Project Settings.


#include "BTMakeDrink.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/AICon.h"

EBTNodeResult::Type UBTMakeDrink::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (UBarRequest* MyRequest = CurrentNPC->CurrentBarRequest)
			{
				float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyRequest->Bar->GetActorLocation());

				if (Distance < 300)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Making drink: %s for request: %d"), *MyRequest->DrinkClass->GetName(), MyRequest->GetUniqueID());
					MyRequest->DrinkActual = CurrentNPC->MakeDrink(MyRequest->DrinkClass, CurrentNPC->CurrentIngredients);
					//UE_LOG(LogTemp, Warning, TEXT("Made drink: %s for request: %d"), *MyRequest->DrinkActual->GetName(), MyRequest->GetUniqueID());
					return EBTNodeResult::Succeeded; 
				}
			}
		}
	}
	return EBTNodeResult::Failed; 
}

void UBTMakeDrink::DrinkCompleted(UBehaviorTreeComponent& OwnerComp)
{
}

void UBTMakeDrink::DrinkFailed(UBehaviorTreeComponent& OwnerComp)
{
}

void UBTMakeDrink::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
