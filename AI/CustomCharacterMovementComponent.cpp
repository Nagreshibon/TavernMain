// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"

void UCustomCharacterMovementComponent::OnCharacterStuckInGeometry(const FHitResult* Hit)
{
	Super::OnCharacterStuckInGeometry(Hit);

	GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + FVector(0, 0, 20)); 
}
