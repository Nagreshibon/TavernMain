// Fill out your copyright notice in the Description page of Project Settings.


#include "BarRequest.h"

UBarRequest::UBarRequest()
{
	
}

UBarRequest::UBarRequest(TSubclassOf<ADrink> inDrink, ANPC* inClient, float inExpTime, ERequestPriority inPriority)
{
	DrinkClass = inDrink;
	Client = inClient;
	ExpirationTime = inExpTime;
	Priority = inPriority; 
}

UBarRequest::~UBarRequest()
{
	if (DrinkActual)
		DrinkActual->GameplayTags.AddTag(FGameplayTag::RequestGameplayTag("ORPHANED"));
}
