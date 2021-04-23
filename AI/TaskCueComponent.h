// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BehaviorTree.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TaskCueComponent.generated.h"

UENUM(BlueprintType)
enum class ENPCTaskPriority : uint8
{
	MAXIMUM = 0    UMETA(DisplayName = "Maximum"),
	URGENT UMETA(DisplayName = "Urgent"),
	NORMAL UMETA(DisplayName = "Normal"),
	LOW UMETA(DisplayName = "Low")
};

UENUM(BlueprintType)
enum class ENPCTaskType : uint8
{
	IDLE = 0    UMETA(DisplayName = "Idle"),
	CHAT UMETA(DisplayName = "Chat"),
	SITGROUP UMETA(DisplayName = "Sit With Group"),
	SITANDCHAT UMETA(DisplayName = "Sit Social"),
	SITALONE UMETA(DisplayName = "Sit Alone"),
	DRINKBAR UMETA(DisplayName = "Drink at bar"),
	BARTEND UMETA(DisplayName = "Bartend"),
	CLEAN UMETA(DisplayName = "Clean"),
	WAITRESS UMETA(DisplayName = "Serve"),
	COOK UMETA(DisplayName = "Cook"),
	GUARD UMETA(DisplayName = "Guard"),
	BRAWL UMETA(DisplayName = "Brawl"),
};

USTRUCT(BlueprintType)
struct FNPCTask
{
	GENERATED_BODY()
	
	ENPCTaskType Task; 
	ENPCTaskPriority Priority;
	float ExpirationTime;
	UPROPERTY()
	AActor* Target = nullptr;

	FORCEINLINE bool operator< (const FNPCTask& rhs) const { return Priority < rhs.Priority; } 
	//inline bool operator< (const FNPCTask& lhs, const FNPCTask& rhs) { return lhs.Priority < rhs.Priority; }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAV_API UTaskCueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTaskCueComponent();

	FORCEINLINE TArray<FNPCTask>* GetTaskArray() { return &TaskArr; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_Idle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_Chat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_SitGroup;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_SitAndChat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_SitAlone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_DrinkBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_Bartend;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_Clean;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_Waitress;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_Cook;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_Guard;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UBehaviorTree> BT_Brawl;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<FNPCTask> TaskArr;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
