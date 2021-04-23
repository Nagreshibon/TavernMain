// Fill out your copyright notice in the Description page of Project Settings.


#include "BTPickupDrink.h"

#include "TAV/AICon.h"
#include "TAV/NPC.h"

EBTNodeResult::Type UBTPickupDrink::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			//UAIBlueprintHelperLibrary::SimpleMoveToActor(CurrentNPC->GetController(), CurrentNPC->CurrentDrink);
			CurrentNPC->AnimState = ENPCAnimState::BARPICKUP;

			FTimerDelegate PickupDelegate = FTimerDelegate::CreateLambda([=, &OwnerComp]()
			{
				PickupRelease(OwnerComp);
			});
			GetWorld()->GetTimerManager().SetTimer(UnusedHandle, PickupDelegate, 1.5f, false);

			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTPickupDrink::PickupRelease(UBehaviorTreeComponent& OwnerComp)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			CurrentNPC->AnimState = ENPCAnimState::IDLE;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return; 
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Pickup release failing"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}