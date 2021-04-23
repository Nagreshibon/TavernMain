// Fill out your copyright notice in the Description page of Project Settings.


#include "PropKitchenCounter.h"
#include "Components/BoxComponent.h"
#include "TAV/Requests/TableRequest.h"

APropKitchenCounter::APropKitchenCounter()
{
	Countertop = CreateDefaultSubobject<UBoxComponent>("Countertop");
	Countertop->SetupAttachment(StaticMesh); 
}

UTableRequest* APropKitchenCounter::GetTopRequest()
{
	if (RequestsCue.Num() != 0)
	{
		RequestsCue.Sort();
		for (auto &o : RequestsCue)
		{
			if (!o->Cook) return o; 
		}

		//return OrderCue[0];
	}

	return nullptr;
}

void APropKitchenCounter::RemoveCompletedRequest(UTableRequest* inRequest)
{
	RequestsCompleted.Remove(inRequest);

	if(RequestsCompleted.Num() == 0) GameplayTags.RemoveTag(FGameplayTag::RequestGameplayTag("HASCOMPLETEDORDERS"));
}

void APropKitchenCounter::CompleteRequest(UTableRequest* inRequest)
{
	RequestsCompleted.Add(inRequest);
	GameplayTags.AddTag(FGameplayTag::RequestGameplayTag("HASCOMPLETEDORDERS")); 
}

UTableRequest* APropKitchenCounter::GetTopCompletedRequest()
{
	if (RequestsCompleted.Num() > 0)
	{
		RequestsCompleted.Sort();

		for (auto &r : RequestsCompleted)
		{
			if (r->EdibleActual) return r;
		}

	}

	return nullptr; 
	
}


