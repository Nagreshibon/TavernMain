// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatModifier.h"
#include "TAV/GameplayEffect.h"
#include "CombatComponent.generated.h"


USTRUCT(BlueprintType)
struct FStat
{
	GENERATED_BODY();
	
	float Current;
	float Base;

	FStat() : Current(10), Base(10)
	{
		
	}
	
	FStat(float inBase) : Current(inBase), Base(inBase)
	{
	
	}	
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	PHYSICAL = 0 UMETA(DisplayName = "Physical"),
	MAGIC		 UMETA(DisplayName = "Magic"),
	TRUEDMG		 UMETA(DisplayName = "True"),
	
};

class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAV_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	//stats

	UFUNCTION(BlueprintCallable)
		float ModifyStat(FStat& inStat, float inAmount, float inDuration);

	float ModRelease(FStat& inStat, float inMod);

	TArray<UStatModifier*> ActiveMods;
	TArray<UStatModifier*> DeadMods;
	
	FTimerHandle ModHandle; 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseStats")
		FStat HP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseStats")
		FStat MovementSpeed; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseStats")
		FStat Armor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseStats")
		FStat MagicResistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseStats")
		FStat DamageReduction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AutoAttack")
		FStat AttackSpeed; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AutoAttack")
		FStat AttackRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AutoAttack")
		bool bMelee;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AutoAttack")
		FStat BaseAttackDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AutoAttack")
		EDamageType AutoAttackDamageType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AutoAttack")
		TArray<UAnimMontage*> AutoAttackAnimations; 

	UPROPERTY(BlueprintReadWrite, Category = "AutoAttack")
		TArray<UGameplayEffect*> OnHitArray; 
	
	UPROPERTY()
	AActor* CurrentAttackTarget;

	UFUNCTION(BlueprintCallable)
		void TestModifier();
	
	void AddModifier(FStat& inStat, float inAmount, float inDuration);
	
	void RemoveModifier(UStatModifier* inModifier);
	void RemoveDeadMods();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
