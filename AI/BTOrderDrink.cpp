// Fill out your copyright notice in the Description page of Project Settings.


#include "BTOrderDrink.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"
#include "TAV/Props/PropBar.h"

UBTOrderDrink::UBTOrderDrink(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	//NodeName = "Order Drink";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTOrderDrink::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropBar* MyBar = Cast<APropBar>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyBar->GetActorLocation());

					if (Distance < 250)
					{
						CurrentBar = MyBar; 
						CurrentRequest = MyBar->AddRequest(CurrentNPC->PickYourPoison(), CurrentNPC, 30, ERequestPriority::NORMAL);
						UE_LOG(LogTemp, Warning, TEXT("Making an order! NPC: %s, ID: %d"), *CurrentNPC->GetName(), CurrentRequest->GetUniqueID());
						AICon->BlackboardComp->SetValueAsObject("CurrentBarRequest", CurrentRequest); 
						ExpirationTimer = 45;
						if (UBarRequest* Request = Cast<UBarRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentBarRequest")))
						{
							CurrentNPC->SayDrinkLine(Request->DrinkClass, EDrinkLineType::ORDER);
						}
						return EBTNodeResult::InProgress;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Bar too far!"));
					}
			}
		}
	}
	return EBTNodeResult::Failed; 
}

void UBTOrderDrink::OrderCompleted(UBehaviorTreeComponent& OwnerComp)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (UBarRequest* Request = Cast<UBarRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentBarRequest")))
		{
			if (APropBar* MyBar = Cast<APropBar>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
			{
				DrinkClient = Request->Client;
				DrinkToDestroy = Request->DrinkActual;
				FTimerDelegate DrinkKillDelegate = FTimerDelegate::CreateLambda([=, &OwnerComp]()
				{
					AttachDrink(DrinkToDestroy, DrinkClient, OwnerComp);
				});
				GetWorld()->GetTimerManager().SetTimer(UnusedHandle, DrinkKillDelegate, 1.0f, false);

				if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
				{
					CurrentNPC->CurrentDrink = DrinkToDestroy;
					AICon->BlackboardComp->SetValueAsObject("CurrentDrink", DrinkToDestroy);
				}

				//UE_LOG(LogTemp, Warning, TEXT("Request for: %s removed"), *Request->DrinkClass->GetDefaultObject()->GetName());
				MyBar->RemoveRequest(Request);
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Order completed, client happy."));
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
}

void UBTOrderDrink::AttachDrink(ADrink* inDrink, ANPC* inClient, UBehaviorTreeComponent& OwnerComp)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			
		}
	}
	//FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false); ;
	//inDrink->AttachToComponent(inClient->GetMesh(), TransformRules, TEXT("hand_r"));
}

void UBTOrderDrink::KillDrink(ADrink* inDrink)
{
	//inDrink->Destroy(); 
}

void UBTOrderDrink::OrderFailed(UBehaviorTreeComponent& OwnerComp)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (APropBar* MyBar = Cast<APropBar>(AICon->BlackboardComp->GetValueAsObject("CurrentTarget")))
		{
			if (UBarRequest* Request = Cast<UBarRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentBarRequest")))
			{
				UE_LOG(LogTemp, Warning, TEXT("Request for: %s removed"), *Request->DrinkClass->GetDefaultObject()->GetName());
				if (Request->DrinkActual) Request->DrinkActual->GameplayTags.AddTag(FGameplayTag::RequestGameplayTag("ORPHANED"));
				MyBar->RemoveRequest(Request);
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Order failed, client unhappy."));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed); 
}

void UBTOrderDrink::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	//UE_LOG(LogTemp, Warning, TEXT("BTOrderDrink Tick: %f"), ExpirationTick);
	if(ExpirationTimer>ExpirationTick)
	{
		ExpirationTick += DeltaSeconds;
		
		
		if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
		{
			if (UBarRequest* Request = Cast<UBarRequest>(AICon->BlackboardComp->GetValueAsObject("CurrentBarRequest")))
			{
				if (Request->bInvalidated) 
				{
					ExpirationTimer = 0;
					ExpirationTick = 0;
					OrderFailed(OwnerComp);
				}
				//UE_LOG(LogTemp, Warning, TEXT("Checking order ID: %d"), Request->GetUniqueID());
				if (Request->DrinkActual)
				{
					//UE_LOG(LogTemp, Warning, TEXT("DrinkActual detected"));
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
