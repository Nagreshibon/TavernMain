// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AICon.generated.h"


/**
 * 
 */
UCLASS()
class TAV_API AAICon : public AAIController
{
	GENERATED_BODY()

		AAICon(); 
	
	public:
		virtual void OnPossess(APawn* InPawn) override;
		void SetCurrentTarget(AActor* InTarget);
		void SetPlayerCharacter(APawn* PlayerCharacter);

		UPROPERTY(EditDefaultsOnly, Category = "AI")
			FName CurrentTargetKey;

		UFUNCTION(BlueprintCallable)
			void RestartTree();

		UFUNCTION(BlueprintCallable)
			void SetTree(UBehaviorTree* inTree);
	
	UPROPERTY()
		UBehaviorTreeComponent* BehaviorComp;

	UFUNCTION(BlueprintCallable)
		UBlackboardComponent* ReturnBlackboard_Blueprints() { return BlackboardComp; }
	
	UPROPERTY()
		UBlackboardComponent* BlackboardComp;

		FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

		//FORCEINLINE TArray<AActor*> GetAvailableTargetPoints() { return BotTargetPoints; }
	
};
