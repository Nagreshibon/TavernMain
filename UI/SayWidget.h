// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "SayWidget.generated.h"

/**
 * 
 */
UCLASS()
class TAV_API USayWidget : public UUserWidget
{
	GENERATED_BODY()

public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* NameBlock;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UTextBlock* SayBlock;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UImage* PortraitImage; 
};
