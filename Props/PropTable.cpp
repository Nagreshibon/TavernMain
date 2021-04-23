// Fill out your copyright notice in the Description page of Project Settings.


#include "PropTable.h"
#include "PropChair.h"
#include "Components/BoxComponent.h"

APropTable::APropTable()
{
	ChairSlots = 4;
	SetActorTickInterval(1.f);

	Tabletop = CreateDefaultSubobject<UBoxComponent>(TEXT("Tabletop"));
	Tabletop->SetupAttachment(StaticMesh); 
}

int APropTable::GetNumOccupiedChairs()
{
	int i = 0;
	for (auto &c : ChairArray)
	{
		if (c->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("OCCUPIED")))
		{
			i++; 
		}
	}

	return i; 
}

TArray<UTableRequest*> APropTable::GetRequests()
{
	return RequestArr; 
}

UTableRequest* APropTable::AddRequest(TSubclassOf<AEdible> inEdibleClass, ANPC* inClient, float inExpTime,
                                      ERequestPriority inPriority)
{
	UTableRequest* NewRequest = NewObject<UTableRequest>();
	NewRequest->EdibleClass = inEdibleClass;
	NewRequest->Client = inClient;
	NewRequest->Table = this;
	NewRequest->ExpirationTime = inExpTime;
	NewRequest->Priority = inPriority;
	int32 idx = RequestArr.Add(NewRequest);
	GameplayTags.AddTag(FGameplayTag::RequestGameplayTag("HASORDERS"));
	if (GameplayTags.HasTag(FGameplayTag::RequestGameplayTag("HASORDERS")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Order: %d, HASORDERS added"), NewRequest->GetUniqueID());
	}
	return NewRequest;
}

bool APropTable::RemoveRequest(UTableRequest* inRequest)
{
	if (RequestArr.Remove(inRequest) != 0)
	{
		if(RequestArr.Num() == 0) GameplayTags.RemoveTag(FGameplayTag::RequestGameplayTag("HASORDERS"));
		return true;
	}

	return false;
}

void APropTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}
