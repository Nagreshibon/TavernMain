// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Prop.h"
#include "PropTable.h"
#include "PropChair.generated.h"


/**
 * 
 */
UCLASS()
class TAV_API APropChair : public AProp
{
	GENERATED_BODY()

		APropChair(); 
	
		bool PostOverlap(UPrimitiveComponent* Comp, AActor* OtherActor) override;

		bool PostEndOverlap(UPrimitiveComponent* Comp, AActor* OtherActor) override;


public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* SitMarker; 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		APropTable* OwningTable;

	void BeginPlay() override;
	
};
