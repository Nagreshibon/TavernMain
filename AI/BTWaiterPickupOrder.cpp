// Fill out your copyright notice in the Description page of Project Settings.


#include "BTWaiterPickupOrder.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/Props/PropKitchenCounter.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"

EBTNodeResult::Type UBTWaiterPickupOrder::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropKitchenCounter* MyKitchenCounter = Cast<APropKitchenCounter>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				UTableRequest* Request = MyKitchenCounter->GetTopCompletedRequest();
				if (Request)
				{
					CurrentNPC->AnimState = ENPCAnimState::BARPICKUP;
					CurrentNPC->CurrentTableRequests.Add(Request);
					MyKitchenCounter->RemoveCompletedRequest(Request); 
					AICon->BlackboardComp->SetValueAsObject("CurrentTableRequest", Request); 

					FTimerDelegate PickupDelegate = FTimerDelegate::CreateLambda([=, &OwnerComp]()
					{
						PickupRelease(OwnerComp);
					});
					GetWorld()->GetTimerManager().SetTimer(UnusedHandle, PickupDelegate, 2.7f, false);

					return EBTNodeResult::InProgress;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTWaiterPickupOrder::PickupRelease(UBehaviorTreeComponent& OwnerComp)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (UTableRequest* Request = Cast<UTableRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentTableRequest")))
			{
				if (Request->EdibleActual)
				{
					CurrentNPC->AnimState = ENPCAnimState::IDLE;
					CurrentNPC->Carry(Request->EdibleActual);
					AICon->BlackboardComp->SetValueAsObject("CurrentTarget", Request->Table); 
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return;
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Pickup release failing"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}