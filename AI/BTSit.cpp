// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TAV/AICon.h"
#include "TAV/NPC.h"
#include "TAV/Props/PropChair.h"


EBTNodeResult::Type UBTSit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			if (APropChair* MyChair = Cast<APropChair>(AICon->BlackboardComp->GetValueAsObject("CurrentChair")))
			{
				if (!MyChair->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("OCCUPIED"))))
				{
					float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), MyChair->GetActorLocation());

					if (Distance < 200)
					{
						FTimerDelegate SitReleaseDelegate = FTimerDelegate::CreateLambda([=, &OwnerComp]()
						{
							SitRelease(OwnerComp);
						});
						GetWorld()->GetTimerManager().SetTimer(UnusedHandle, SitReleaseDelegate, 1.5f, false);

						CurrentNPC->SetActorLocation(MyChair->SitMarker->GetComponentLocation(), false);
						CurrentNPC->SetActorRotation(MyChair->SitMarker->GetComponentRotation());
						CurrentNPC->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap); 
						CurrentNPC->AnimState = ENPCAnimState::STANDTOSIT;
						AICon->BlackboardComp->SetValueAsBool("bIsSitting", true);

						MyChair->GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("OCCUPIED")));
						CurrentNPC->CurrentChair = MyChair; 

						return EBTNodeResult::InProgress;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Chair too far!"));
					}
				}
				
			}			
		}
	}
	return EBTNodeResult::Failed;
}

void UBTSit::SitRelease(UBehaviorTreeComponent& OwnerComp)
{
	if (AAICon* AICon = Cast<AAICon>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* CurrentNPC = Cast<ANPC>(AICon->GetPawn()))
		{
			CurrentNPC->AnimState = ENPCAnimState::SITIDLE;
			CurrentNPC->bIsSitting = true; 
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}
