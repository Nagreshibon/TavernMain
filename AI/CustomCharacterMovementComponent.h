// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TAV_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

		virtual void OnCharacterStuckInGeometry(const FHitResult* Hit) override;
	
};
