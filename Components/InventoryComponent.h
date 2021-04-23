// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TAV/Ingredient.h"
#include "InventoryComponent.generated.h"


class AProp; 


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAV_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<AProp*> InventoryArr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Gold;

	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool CanAfford(float inPrice) { return inPrice <= Gold; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
