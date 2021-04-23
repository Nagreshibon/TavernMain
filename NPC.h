// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Combat/CombatComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Props/Drink.h"
#include "GameFramework/Character.h"
#include "Props/PropChair.h"
#include "Requests/BarRequest.h"
#include "Props/PropStorage.h"
#include "Dialogue/DialogueBody.h"
#include "NPC.generated.h"



UENUM(BlueprintType)
enum class ENPCAnimState : uint8 {
	IDLE = 0    UMETA(DisplayName = "Idle"),
	STANDTOSIT UMETA(DisplayName = "Standing to Sitting"),
	SITTOSTAND UMETA(DisplayName = "Sitting to Standing"),
	SITIDLE    UMETA(DisplayName = "Sitting Idle"),
	SITANGRY    UMETA(DisplayName = "Sitting Angry"),
	SITCHEERING    UMETA(DisplayName = "Sitting Cheering"),
	SITFEMALE1    UMETA(DisplayName = "Sitting Female 1"),
	SITFEMALE2	UMETA(DisplayName = "Sitting Female 2"),
	SITCLAP	UMETA(DisplayName = "Sitting Clap"),
	SITDISBELIEF	UMETA(DisplayName = "Sitting Disbelief"),
	SITDISBELIEF2	UMETA(DisplayName = "Sitting Disbelief 2"),
	SITRUBBING	UMETA(DisplayName = "Sitting Rubbing"),
	DRINK1	UMETA(DisplayName = "Drinking 1"),
	DRINK2	UMETA(DisplayName = "Drinking 2"),
	BARPICKUP UMETA(DisplayName = "Pick Up From Bar"),
	CARRYING UMETA(DisplayName = "Carrying"),
	PUTDOWN UMETA(DisplayName = "Putting Down"),
	CLEANFLOOR UMETA(DisplayName = "Clean Floor"), 
};

//enum class EDrinkLineType : uint8;

UCLASS()
class TAV_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	//combat
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCombatComponent* CombatComponent; 

	//inventory
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInventoryComponent* Inventory;

	UFUNCTION(BlueprintCallable)
		void Carry(AProp* inProp);

	UPROPERTY()
	AProp* CarriedItem;

	UFUNCTION(BlueprintCallable)
		AProp* PutDown(AProp* TargetProp, ANPC* inClient = nullptr);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* Broom;

	UFUNCTION(BlueprintCallable)
		void ShowBroom(bool inBool);
	
	

	//widget handling

	UFUNCTION(BlueprintCallable)
		void ShowNameFloater();

	UFUNCTION(BlueprintCallable)
		void HideNameFloater();
	
	void Say(FDrinkLines inDrinkLine);

	UFUNCTION(BlueprintCallable)
	void Say(FText inText, float Duration, USoundBase* SFX);

	UFUNCTION(BlueprintCallable)
		void SayDrinkLine(TSubclassOf<ADrink> inDrinkClass, EDrinkLineType inDrinkLineType);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UDataTable* DrinkLineData; 

	FTimerHandle UnusedHandle; 

	//AUDIO
	
	UPROPERTY(VisibleAnywhere)
		UAudioComponent* AudioComp;

	//BASIC INFO

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName IDName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* Portrait; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UUserWidget> SayWidget_BP; 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UWidgetComponent* SayWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UUserWidget> NameWidget_BP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UWidgetComponent* NameWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//INTERRUPT TASK FLAG -> INTERRUPT THE CURRENT TASK ON NEXT TICK
	bool bInterruptCurrentTask; 
	
	//BARTENDING
	UPROPERTY()
	TArray<TSubclassOf<AIngredient>> CurrentIngredients;
	
	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<ADrink> DrinkToMake;

	UPROPERTY(BlueprintReadOnly)
		UBarRequest* CurrentBarRequest;

	UFUNCTION(BlueprintCallable)
		TArray<TSubclassOf<AIngredient>> CollectIngredients(UBarRequest* inRequest, APropStorage* inStorage);

	UFUNCTION(BlueprintCallable)
		ADrink* MakeDrink(TSubclassOf<ADrink> inDrinkClass, TArray<TSubclassOf<AIngredient>> inIngredients);
	
	//DRINKING

	UPROPERTY(BlueprintReadOnly)
		ADrink* CurrentDrink; 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bDrinkRequestFulfilled = false;

	FORCEINLINE bool GetDrinkRequestFulfilled() const { return bDrinkRequestFulfilled; }
	
	UFUNCTION(BlueprintCallable)
		void DrinkRequestFulfilled(ADrink* inDrink); 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<TSubclassOf<ADrink>> FavouriteDrinks;

	UFUNCTION(BlueprintCallable)
		TSubclassOf<ADrink> PickYourPoison();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<ADrink> DefaultDrink;

	//EATING

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<TSubclassOf<AEdible>> FavouriteMeals;

	UFUNCTION(BlueprintCallable)
		TSubclassOf<AEdible> MealPicker();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<AEdible> DefaultMeal;

	//SERVING

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<UTableRequest*> CurrentTableRequests;

	//COOKING
	UPROPERTY(BlueprintReadWrite)
		UTableRequest* CurrentKitchenRequest;
	
		TArray<TSubclassOf<AIngredient>> CollectIngredients(UTableRequest* inRequest, APropStorage* inStorage);

		AEdible* MakeMeal(TSubclassOf<AEdible> inEdibleClass, TArray<TSubclassOf<AIngredient>> inIngredients);

	//SITTING / GETTING UP
	
	UFUNCTION(BlueprintCallable)
		void GetUp(); 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsSitting = false;

	UFUNCTION(BlueprintCallable)
		void InterruptTask();


	UPROPERTY()
		APropChair* CurrentChair; 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* CurrentTarget; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ENPCAnimState> AnimState;*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ENPCAnimState AnimState; 
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
		FTimerHandle Handle1; 

};


