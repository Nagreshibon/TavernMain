// Fill out your copyright notice in the Description page of Project Settings.


#include "BTGetUp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"

EBTNodeResult::Type UBTGetUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			CurrentNPC->GetUp();
			FTimerDelegate GetUpReleaseDelegate = FTimerDelegate::CreateLambda([=, &OwnerComp]()
			{
				GetUpRelease(OwnerComp);
			});
			GetWorld()->GetTimerManager().SetTimer(UnusedHandle, GetUpReleaseDelegate, 1.5f, false);
			return EBTNodeResult::InProgress;
			
		}

	}
	return EBTNodeResult::Failed; 

}

void UBTGetUp::GetUpRelease(UBehaviorTreeComponent& OwnerComp)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			CurrentNPC->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
			CurrentNPC->AnimState = ENPCAnimState::IDLE;
			CurrentNPC->bIsSitting = false;
			AICon->BlackboardComp->SetValueAsBool("bIsSitting", false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}