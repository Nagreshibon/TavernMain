// Fill out your copyright notice in the Description page of Project Settings.


#include "AICon.h"


#include "Kismet/GameplayStatics.h"
#include "NPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AAICon::AAICon()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AAICon::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ANPC* AIChar = Cast<ANPC>(InPawn))
	{

		if (AIChar)
		{
			if (AIChar->BehaviorTree)
			{
				if (AIChar->BehaviorTree->BlackboardAsset)
				{
					BlackboardComp->InitializeBlackboard(*(AIChar->BehaviorTree->BlackboardAsset));
				}

				//location finder
				//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestTargetPoint::StaticClass(), BotTargetPoints);
				//UE_LOG(LogTemp, Warning, TEXT("BTNode: Looking for TargetPoints with tag: %s"), *AIChar->IDName);
				/*if (*AIChar->IDName)
				{
					UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATestTargetPoint::StaticClass(), FName(*AIChar->IDName), BotTargetPoints);
				}*/

				if (AIChar->BehaviorTree)
				{
					BehaviorComp->StartTree(*AIChar->BehaviorTree);
				}

			}
		}
	}
}

void AAICon::SetCurrentTarget(AActor* InTarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject("CurrentTarget", InTarget);
		if (ANPC* MyNPC = Cast<ANPC>(GetPawn()))
		{
			MyNPC->CurrentTarget = InTarget;
		}
		//UE_LOG(LogTemp, Warning, TEXT("SETTING TARGET ENEMY KEY TO PAWN"));
	}
}

void AAICon::SetPlayerCharacter(APawn* PlayerCharacter)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject("PlayerCharacter", PlayerCharacter);
		//UE_LOG(LogTemp, Warning, TEXT("SETTING PLAYER CHARACTER KEY ON THE CONTROLLER"));
	}
}

void AAICon::RestartTree()
{
	BehaviorComp->RestartTree(); 
}

void AAICon::SetTree(UBehaviorTree* inTree)
{
	BehaviorComp->StartTree(*inTree);
}