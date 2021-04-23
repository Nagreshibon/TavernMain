// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

float UCombatComponent::ModifyStat(FStat& inStat, float inAmount, float inDuration)
{
	inStat.Current += inAmount; 
	
	FTimerDelegate ModReleaseDelegate = FTimerDelegate::CreateLambda([=, &inAmount, &inStat]()
	{
		ModRelease(inStat, inAmount);
	});
	GetWorld()->GetTimerManager().SetTimer(ModHandle, ModReleaseDelegate, inDuration, false);
	
	return inStat.Current;
}

float UCombatComponent::ModRelease(FStat& inStat, float inMod)
{
	inStat.Current += -inMod;

	return inStat.Current; 
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

void UCombatComponent::TestModifier()
{
	AddModifier(HP, 12, 2.5);
}

void UCombatComponent::AddModifier(FStat& inStat, float inAmount, float inDuration)
{
	auto Modifier = new UStatModifier(inStat, inAmount, inDuration, *this); 
	//ActiveMods.Add(Modifier); 
	ActiveMods.Emplace(Modifier);
}

void UCombatComponent::RemoveModifier(UStatModifier* inModifier)
{
	DeadMods.Add(inModifier); 
}

void UCombatComponent::RemoveDeadMods()
{
	for(auto &m : DeadMods)
	{
		ActiveMods.Remove(m); 
		auto temp = m;
		delete temp; 
	}
	DeadMods.Empty(); 
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ActiveMods.Num() > 0)
	{
		for (auto& m : ActiveMods)
		{
			m->Tick(DeltaTime);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Test HP mod: %f"), HP.Current);
	
	RemoveDeadMods(); 
	
	// ...
}

