// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCleanFloor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/AICon.h"
#include "TAV/Misc/DirtDecal.h"
#include "TAV/NPC.h"

EBTNodeResult::Type UBTCleanFloor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (ADirtDecal* MyDecal = Cast<ADirtDecal>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyDecal->GetActorLocation());

				if (Distance < 250)
				{
					CurrentNPC->AnimState = ENPCAnimState::CLEANFLOOR;
					CurrentNPC->ShowBroom(true); 
					FTimerDelegate CleanFloorDelegate = FTimerDelegate::CreateLambda([=, &OwnerComp]()
					{
						CleanFloorRelease(OwnerComp);
					});
					GetWorld()->GetTimerManager().SetTimer(UnusedHandle, CleanFloorDelegate, 2.5f, false);

					return EBTNodeResult::InProgress; 
					
					
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTCleanFloor::CleanFloorRelease(UBehaviorTreeComponent& OwnerComp)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (ADirtDecal* MyDecal = Cast<ADirtDecal>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				if (MyDecal->Destroy())
				{
					CurrentNPC->AnimState = ENPCAnimState::IDLE;
					CurrentNPC->ShowBroom(false); 
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return; 
				}
			}
			CurrentNPC->ShowBroom(false);
			CurrentNPC->AnimState = ENPCAnimState::IDLE;
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}
