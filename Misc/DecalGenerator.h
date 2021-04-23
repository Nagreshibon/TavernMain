// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DirtDecal.h"
#include "Components/SphereComponent.h"
#include "DecalGenerator.generated.h"


UCLASS()
class TAV_API ADecalGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecalGenerator();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<TSubclassOf<ADirtDecal>> DecalClasses;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USphereComponent* SphereComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bActive = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Interval;

	UPROPERTY()
		float Ticker; 

	UFUNCTION(BlueprintCallable)
		ADirtDecal* SpawnDecal(); 
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
