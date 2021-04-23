// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"


#include "AI/CustomCharacterMovementComponent.h"
#include "AICon.h"
#include "UI/NameWidget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "Props/PropBar.h"
#include "TAVPlayerController.h"
#include "Components/AudioComponent.h"
//#include "Dialogue/DialogueBody.h"
#include "Kismet/KismetMathLibrary.h"
#include "Props/PropStove.h"
#include "UI/SayWidget.h"
#include "Props/PropKitchenCounter.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NameWidget = CreateDefaultSubobject<UWidgetComponent>(FName("NameWidget"));
	NameWidget->SetupAttachment(RootComponent);
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	NameWidget->SetHiddenInGame(true);

	SayWidget = CreateDefaultSubobject<UWidgetComponent>(FName("SayWidget"));
	SayWidget->SetupAttachment(RootComponent);
	SayWidget->SetWidgetSpace(EWidgetSpace::Screen);
	SayWidget->SetHiddenInGame(true);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);

	Inventory = CreateDefaultSubobject<UInventoryComponent>(FName("Inventory"));
	//Inventory->SetupAttachment(RootComponent);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(FName("Combat Component"));

	auto cm = GetCharacterMovement();
	cm->SetMovementMode(EMovementMode::MOVE_NavWalking);

	Broom = CreateDefaultSubobject<UStaticMeshComponent>(FName("Broom"));
	Broom->SetupAttachment(GetMesh(), "hand_r");
	Broom->SetHiddenInGame(true);
	Broom->SetVisibility(false); 
	
}

void ANPC::Carry(AProp* inProp)
{
	AnimState = ENPCAnimState::CARRYING;
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false); ;
	inProp->AttachToComponent(GetMesh(), TransformRules, TEXT("index_03_r"));
	//inProp->SetActorRelativeLocation(FVector(0, 0, 0)); 

	if (AEdible* MyEdible = Cast<AEdible>(inProp))
	{
		MyEdible->SetActorRelativeTransform(MyEdible->SocketTransform);
		MyEdible->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	}

	//inProp->SetActorRotation(FRotator(0,0,0));

	CarriedItem = inProp; 
}

AProp* ANPC::PutDown(AProp* TargetProp, ANPC* inClient)
{
	UBoxComponent* Top = nullptr;
	
	if(APropTable* MyTable = Cast<APropTable>(TargetProp))
	{
		Top = MyTable->Tabletop; 
	}
	else if (APropBar* MyBar = Cast<APropBar>(TargetProp))
	{
		Top = MyBar->Countertop;
	}
	else if (APropKitchenCounter* MyKitchenCounter = Cast<APropKitchenCounter>(TargetProp))
	{
		Top = MyKitchenCounter->Countertop;
	}
	else if (APropStove* MyStove = Cast<APropStove>(TargetProp))
	{
		Top = MyStove->Stovetop; 
	}

	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, false);
	
	if (Top)
	{
		FVector RandomPoint;
		if (inClient)
		{
			FVector R = inClient->GetActorLocation() - Top->GetComponentLocation();
			FVector RNorm = R;
			RNorm.Normalize();
			float Cos = RNorm.CosineAngle2D(Top->GetRightVector());
			FVector X;
			if (AEdible* MyEdible = Cast<AEdible>(CarriedItem))
			{
				X = Top->GetRightVector() * (Cos * R.Size() - MyEdible->BarOffset);
			}
			else
			{
				X = Top->GetRightVector() * (Cos * R.Size());
			}

			RandomPoint = Top->GetComponentLocation() + X;
		}
		else
		{
			RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Top->GetComponentLocation(), Top->GetScaledBoxExtent().RotateAngleAxis(Top->GetComponentRotation().Yaw, FVector(0, 0, 1)));
		}

		
		CarriedItem->DetachFromActor(DetachmentRules);
		CarriedItem->SetActorLocation(RandomPoint);
		CarriedItem->SetActorRotation(FRotator(0, UKismetMathLibrary::RandomFloatInRange(0,360), 0)); 
		CarriedItem->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 

		AnimState = ENPCAnimState::IDLE;
		return CarriedItem;
	}
	CarriedItem->DetachFromActor(DetachmentRules);
	CarriedItem->SetActorRotation(FRotator(0, UKismetMathLibrary::RandomFloatInRange(0, 360), 0));
	CarriedItem->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	AnimState = ENPCAnimState::IDLE;
	return nullptr; 

}

void ANPC::ShowBroom(bool inBool)
{
	if(inBool)
	{
		Broom->SetVisibility(true); 
		Broom->SetHiddenInGame(false);
		return;
	}
	Broom->SetVisibility(false);
	Broom->SetHiddenInGame(true);
}

void ANPC::ShowNameFloater()
{
	NameWidget->SetHiddenInGame(false);
}

void ANPC::HideNameFloater()
{
	NameWidget->SetHiddenInGame(true);
}

void ANPC::Say(FDrinkLines inDrinkLine)
{
	if (USayWidget* MyWidget = Cast<USayWidget>(SayWidget->GetUserWidgetObject()))
	{
		SayWidget->SetHiddenInGame(false);
		MyWidget->SayBlock->SetText(inDrinkLine.DialogueText);

		FTimerDelegate DrinkKillDelegate = FTimerDelegate::CreateLambda([=]()
		{
			SayWidget->SetHiddenInGame(true);
		});

		float Duration;

		if (inDrinkLine.SFX)
		{
			Duration = inDrinkLine.SFX->GetDuration() + 0.5f;
		}
		else
		{
			if (inDrinkLine.Duration != 0) Duration = inDrinkLine.Duration;
			else Duration = 2.0f; 	
		}

		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, DrinkKillDelegate, Duration, false);
	}
}

void ANPC::Say(FText inText, float inDuration, USoundBase* inSFX = nullptr)
{
	if (USayWidget* MyWidget = Cast<USayWidget>(SayWidget->GetUserWidgetObject()))
	{
		SayWidget->SetHiddenInGame(false); 
		MyWidget->SayBlock->SetText(inText);

		FTimerDelegate DrinkKillDelegate = FTimerDelegate::CreateLambda([=]()
		{
			SayWidget->SetHiddenInGame(true); 
		});

		float Duration;

		if(inSFX)
		{
			Duration = inSFX->GetDuration() + 0.5f; 
		}
		else
		{
			Duration = inDuration; 
		}


		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, DrinkKillDelegate, Duration, false);
	}
}

void ANPC::SayDrinkLine(TSubclassOf<ADrink> inDrinkClass, EDrinkLineType inDrinkLineType)
{
	if (DrinkLineData)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SayDrinkLine called with param: %s"), *inDrinkClass->GetName());
		TArray<FDrinkLines> DrinkLines;
		FString ContextString;
		TArray<FName> RowNames = DrinkLineData->GetRowNames();

		for (auto& name : RowNames)
		{
			if (FDrinkLines* row = DrinkLineData->FindRow<FDrinkLines>(name, ContextString))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Reading line: '%s'"), *(*row).DialogueText.ToString());
				if ((*row).Drink == inDrinkClass && (*row).Type == inDrinkLineType)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Adding line to array."));
					DrinkLines.Add(*row);
				}
			}
		}

		if (DrinkLines.Num() > 0)
		{
			int k = UKismetMathLibrary::RandomIntegerInRange(0, DrinkLines.Num()-1);
			Say(DrinkLines[k]);
		}
	}
	
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	if(SayWidget_BP)
	{
		SayWidget->SetWidgetClass(SayWidget_BP);

		if(USayWidget* MyWidget = Cast<USayWidget>(SayWidget->GetUserWidgetObject()))
		{
			if (Portrait)
			{
				MyWidget->PortraitImage->SetBrushFromTexture(Portrait); 
			}
			else
			{
				MyWidget->PortraitImage->SetVisibility(ESlateVisibility::Hidden);
			}

			MyWidget->NameBlock->SetText(FText::FromString(DisplayName)); 
		}
	}

	if(NameWidget_BP)
	{
		NameWidget->SetWidgetClass(NameWidget_BP);
		
		if(UNameWidget* MyWidget = Cast<UNameWidget>(NameWidget->GetUserWidgetObject()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Setting widget to DisplayName"));
			MyWidget->NameBlock->SetText(FText::FromString(DisplayName)); 
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Name widget cast failed"));
		}
	}

	if(ATAVPlayerController* MyController = Cast<ATAVPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		MyController->NPCManager->NPCSet.Add(this); 
	}
	
}

TArray<TSubclassOf<AIngredient>> ANPC::CollectIngredients(UBarRequest* inRequest, APropStorage* inStorage)
{
	TArray<TSubclassOf<AIngredient>> InIngredients = inRequest->DrinkClass->GetDefaultObject<ADrink>()->Ingredients;
	TArray<TSubclassOf<AIngredient>> OutIngredients;
	if (InIngredients.Num() > 0)
	{
		if (inStorage->bIsInStorage(InIngredients[0]))
		{
			for (auto in : InIngredients)
			{
				if (inStorage->TakeFromStorage(in))
				{
					
					OutIngredients.Add(in);
				}
			}
		}
		CurrentIngredients = OutIngredients; 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No ingredients in the request"));
	}
	return OutIngredients; 
}

TArray<TSubclassOf<AIngredient>> ANPC::CollectIngredients(UTableRequest* inRequest, APropStorage* inStorage)
{
	TArray<TSubclassOf<AIngredient>> InIngredients = inRequest->EdibleClass->GetDefaultObject<AEdible>()->Ingredients;
	TArray<TSubclassOf<AIngredient>> OutIngredients;
	if (InIngredients.Num() > 0)
	{
		if (inStorage->bIsInStorage(InIngredients[0]))
		{
			for (auto in : InIngredients)
			{
				if (inStorage->TakeFromStorage(in))
				{

					OutIngredients.Add(in);
				}
			}
		}
		CurrentIngredients = OutIngredients;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No ingredients in the request"));
	}
	return OutIngredients;
}

ADrink* ANPC::MakeDrink(TSubclassOf<ADrink> inDrinkClass, TArray<TSubclassOf<AIngredient>> inIngredients)
{
	if(APropBar* MyBar = Cast<APropBar>(CurrentTarget))
	{
		if(inDrinkClass.GetDefaultObject()->Ingredients[0] == inIngredients[0])
		{
			//unsigned int BaseQuality = inIngredients.Num();
			float Quality = 0;
			float Strength = 0; 
			
			for (auto k : inIngredients)
			{
				Quality += k->GetDefaultObject<AIngredient>()->Quality;
				Strength += k->GetDefaultObject<AIngredient>()->AddStrength; 
			}
			//UE_LOG(LogTemp, Warning, TEXT("Ingredients processed successfully"));

			ANPC* inClient; 
			if(CurrentBarRequest->Client)
			{
				inClient = CurrentBarRequest->Client;
			}
			else
			{
				inClient = nullptr; 
			}
			CurrentBarRequest = nullptr;
			
			return MyBar->SpawnDrink(inDrinkClass, Strength, Quality, inClient);
		}
		
	}

	return nullptr;
}

AEdible* ANPC::MakeMeal(TSubclassOf<AEdible> inEdibleClass, TArray<TSubclassOf<AIngredient>> inIngredients)
{
	if (APropStove* MyStove = Cast<APropStove>(CurrentTarget))
	{
		if (inEdibleClass.GetDefaultObject()->Ingredients[0] == inIngredients[0])
		{
			//unsigned int BaseQuality = inIngredients.Num();
			float Quality = 0;
			float Strength = 0;

			for (auto k : inIngredients)
			{
				Quality += k->GetDefaultObject<AIngredient>()->Quality;
			}
			//UE_LOG(LogTemp, Warning, TEXT("Ingredients processed successfully"));

			ANPC* inClient;
			if (CurrentKitchenRequest->Client)
			{
				inClient = CurrentKitchenRequest->Client;
			}
			else
			{
				inClient = nullptr;
			}
			CurrentBarRequest = nullptr;

			return MyStove->SpawnEdible(inEdibleClass, Quality, inClient);
		}

	}

	return nullptr;
}

void ANPC::DrinkRequestFulfilled(ADrink* inDrink)
{
	if(AAICon* MyController = Cast<AAICon>(GetController()))
	{
	}
}

TSubclassOf<ADrink> ANPC::PickYourPoison()
{
	if(FavouriteDrinks.Num() != 0) return FavouriteDrinks[0];

	return DefaultDrink; 
}

TSubclassOf<AEdible> ANPC::MealPicker()
{
	if (FavouriteMeals.Num() != 0) return FavouriteMeals[0];

	return DefaultMeal; 
}

void ANPC::GetUp()
{
	bIsSitting = false;
	AnimState = ENPCAnimState::SITTOSTAND;

	FTimerDelegate SitReleaseDelegate = FTimerDelegate::CreateLambda([=]()
	{
		AnimState = ENPCAnimState::IDLE;
		if(CurrentChair)
		{
			CurrentChair->GameplayTags.RemoveTag(FGameplayTag::RequestGameplayTag("OCCUPIED")); 
		}
	});
	GetWorld()->GetTimerManager().SetTimer(Handle1, SitReleaseDelegate, 1.5f, false);
}

void ANPC::InterruptTask()
{
	if (bIsSitting) AnimState = ENPCAnimState::SITTOSTAND; 
	else AnimState = ENPCAnimState::IDLE;
	if(AAICon* MyController = Cast<AAICon>(GetController()))
	{
		MyController->BehaviorComp->RestartTree(); 
	}
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

