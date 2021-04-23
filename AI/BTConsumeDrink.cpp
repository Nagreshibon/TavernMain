// Fill out your copyright notice in the Description page of Project Settings.


#include "BTConsumeDrink.h"

#include "Kismet/KismetMathLibrary.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"

EBTNodeResult::Type UBTConsumeDrink::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("Firing consume drink"));
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			//UAIBlueprintHelperLibrary::SimpleMoveToActor(CurrentNPC->GetController(), CurrentNPC->CurrentDrink);
			CurrentNPC->AnimState = ENPCAnimState::DRINK1;

			FTimerDelegate DrinkKillDelegate = FTimerDelegate::CreateLambda([=, &OwnerComp]()
			{
				DrinkRelease(OwnerComp); 
			});
			float BaseTime = 3.f;
			int Multiplier = UKismetMathLibrary::RandomIntegerInRange(2,9);
			GetWorld()->GetTimerManager().SetTimer(UnusedHandle, DrinkKillDelegate, BaseTime*Multiplier, false);

			return EBTNodeResult::InProgress;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Consume drink failing"));
	return EBTNodeResult::Failed; 
}

void UBTConsumeDrink::DrinkRelease(UBehaviorTreeComponent& OwnerComp)
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
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}