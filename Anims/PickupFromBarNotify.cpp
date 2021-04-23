// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupFromBarNotify.h"

#include "TAV/NPC.h"

void UPickupFromBarNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	UE_LOG(LogTemp, Warning, TEXT("Bar anim notification!"));

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		if(ANPC* CurrentNPC = Cast<ANPC>(MeshComp->GetOwner()))
		{
			if(CurrentNPC->CurrentDrink)
			{
				//CurrentNPC->CurrentDrink->SetActorLocation(CurrentNPC->GetMesh()->GetSocketLocation("hand_r"));
				
				UE_LOG(LogTemp, Warning, TEXT("Drink socket transform: %s"), *CurrentNPC->CurrentDrink->SocketTransform.ToString());
				FAttachmentTransformRules TransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false); ;
				CurrentNPC->CurrentDrink->AttachToComponent(CurrentNPC->GetMesh(), TransformRules, TEXT("hand_r"));
				CurrentNPC->CurrentDrink->SetActorRelativeTransform(CurrentNPC->CurrentDrink->SocketTransform);
				CurrentNPC->CurrentDrink->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
				
			}
		}
	}
	
}
