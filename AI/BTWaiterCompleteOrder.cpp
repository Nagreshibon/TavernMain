// Fill out your copyright notice in the Description page of Project Settings.


#include "BTWaiterCompleteOrder.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"

EBTNodeResult::Type UBTWaiterCompleteOrder::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropTable* MyTable = Cast<APropTable>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				if (UTableRequest* Request = Cast<UTableRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentTableRequest")))
				{
					CurrentNPC->AnimState = ENPCAnimState::PUTDOWN;

					FTimerDelegate PutdownDelegate = FTimerDelegate::CreateLambda([=, &OwnerComp]()
					{
						PutdownRelease(OwnerComp);
					});
					GetWorld()->GetTimerManager().SetTimer(UnusedHandle, PutdownDelegate, 1.6f, false);

					return EBTNodeResult::InProgress;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTWaiterCompleteOrder::PutdownRelease(UBehaviorTreeComponent& OwnerComp)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (UTableRequest* Request = Cast<UTableRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentTableRequest")))
			{
				if (Request->EdibleActual)
				{
					if (APropTable* MyTable = Cast<APropTable>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
					{
						Request->bDelivered = true;
						CurrentNPC->CurrentTableRequests.Remove(Request); 
						CurrentNPC->PutDown(MyTable, Request->Client);
						CurrentNPC->AnimState = ENPCAnimState::IDLE; 
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return;
					}
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Pickup release failing"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}
