// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PickupFromBarNotify.generated.h"

/**
 * 
 */
UCLASS()
class TAV_API UPickupFromBarNotify : public UAnimNotify
{
	GENERATED_BODY()

		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
