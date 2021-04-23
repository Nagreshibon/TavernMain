// Fill out your copyright notice in the Description page of Project Settings.


#include "StatModifier.h"

UStatModifier::UStatModifier(FStat& inStat, float inAmount, float inDuration, UCombatComponent& inOwningComp) : ManagedStat(inStat), Amount(inAmount), Duration(inDuration), OwningComp(inOwningComp)
{
	ManagedStat.Current+= inAmount;
}

UStatModifier::~UStatModifier()
{
	UE_LOG(LogTemp, Warning, TEXT("Modifier expired!"));
	ManagedStat.Current += -Amount; 
}

void UStatModifier::Tick(float inDelta)
{
	Ticker += inDelta;
	if(Ticker < Duration)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ticking the modifier: %f"), Ticker);
	}
	else
	{
		OwningComp.RemoveModifier(this); 
	}
}
