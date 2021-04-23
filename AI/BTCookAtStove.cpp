// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCookAtStove.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"
#include "TAV/Props/PropStove.h"

UBTCookAtStove::UBTCookAtStove(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	bCreateNodeInstance = true;
	bNotifyTick = true; 
}

uint16 UBTCookAtStove::GetInstanceMemorySize() const
{
	return sizeof(FBTCookAtStoveMemory);
}

EBTNodeResult::Type UBTCookAtStove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTCookAtStoveMemory* MyMemory = CastInstanceNodeMemory<FBTCookAtStoveMemory>(NodeMemory);
	MyMemory->ExpirationTimer = 0.f;
	MyMemory->ExpirationTick = 0.f; 
	
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropStove* MyStove = Cast<APropStove>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyStove->GetActorLocation());

				if (Distance < 300)
				{
					CurrentNPC->CurrentTarget = MyStove; 
					MyMemory->ExpirationTimer = 5.f; 
					return EBTNodeResult::InProgress;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Stove too far!"));
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTCookAtStove::OrderCompleted(UBehaviorTreeComponent& OwnerComp)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropStove* MyStove = Cast<APropStove>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				if (UTableRequest* Request = Cast<UTableRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentTableRequest")))
				{
					Request->EdibleActual = CurrentNPC->MakeMeal(Request->EdibleClass, CurrentNPC->CurrentIngredients);

					if(Request->EdibleActual)
					{
						CurrentNPC->Carry(Request->EdibleActual); 
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					}
				}
			}
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTCookAtStove::OrderFailed(UBehaviorTreeComponent& OwnerComp)
{
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}

void UBTCookAtStove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FBTCookAtStoveMemory* MyMemory = CastInstanceNodeMemory<FBTCookAtStoveMemory>(NodeMemory);
	
	if (MyMemory->ExpirationTimer > MyMemory->ExpirationTick)
	{
		MyMemory->ExpirationTick += DeltaSeconds;
		//if (MyMemory->ExpirationTick >= 10) UE_LOG(LogTemp, Warning, TEXT("Above 10!"));

		if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
		{
			if (UTableRequest* Request = Cast<UTableRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentTableRequest")))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Order %d tick: %d / %d"), Request->GetUniqueID(), MyMemory->ExpirationTick, MyMemory->ExpirationTimer);
				if (Request->bInvalidated)
				{
					MyMemory->ExpirationTimer = 0;
					MyMemory->ExpirationTick = 0;
					OrderFailed(OwnerComp);
				}
				//UE_LOG(LogTemp, Warning, TEXT("Checking order ID: %d"), Request->GetUniqueID());
			}
		}
	}
	else
	{
		MyMemory->ExpirationTimer = 0;
		MyMemory->ExpirationTick = 0;
		OrderCompleted(OwnerComp);
	}
}
