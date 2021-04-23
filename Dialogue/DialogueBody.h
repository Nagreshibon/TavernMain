// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "../CustomGameplayEffect.h"
#include "TAV/Props/Drink.h"
#include "DialogueBody.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType)
struct FDialogueBody : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		FString IDName;

	UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
		FText DialogueText;

	UPROPERTY(EditAnywhere)
		class USoundBase* SFX;

	UPROPERTY(EditAnywhere)
		float Duration;

	UPROPERTY(EditAnywhere)
		unsigned int NextIndex;

	UPROPERTY(EditAnywhere)
		bool bEnd;

	UPROPERTY(EditAnywhere)
		TArray<FString> DialogueAdditional;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UCustomGameplayEffect> Effect;

};

UENUM(BlueprintType)
enum class EDrinkLineType : uint8 {
	ORDER = 0    UMETA(DisplayName = "Order"),
	ORDERFAIL UMETA(DisplayName = "Order Failed"),
	BARTEND UMETA(DisplayName = "Bartend"),
	SAVOUR UMETA(DisplayName = "Savour"),
	VILE UMETA(DisplayName = "Vile"),
	STRONG UMETA(DisplayName = "Strong"),
};

USTRUCT(BlueprintType)
struct FDrinkLines : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		EDrinkLineType Type; 
	
		UPROPERTY(EditAnywhere)
		FString IDName;

	UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
		FText DialogueText;

	UPROPERTY(EditAnywhere)
		class USoundBase* SFX;

	UPROPERTY(EditAnywhere)
		float Duration;

	UPROPERTY(EditAnywhere)
		unsigned int NextIndex;

	UPROPERTY(EditAnywhere)
		bool bEnd;

	UPROPERTY(EditAnywhere)
		TArray<FString> DialogueAdditional;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UCustomGameplayEffect> Effect;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ADrink> Drink; 

};

USTRUCT(BlueprintType)
struct FDialoguePicker : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		unsigned int DialogueIndex;

	UPROPERTY(EditAnywhere)
		FString IDName;

	UPROPERTY(EditAnywhere)
		UDataTable* DialogueBody;

	UPROPERTY(EditAnywhere)
		bool bLeftSide;

	UPROPERTY(EditAnywhere)
		bool bCustomCamera;
};