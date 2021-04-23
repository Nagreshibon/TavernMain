// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prop.h"
#include "PropSpawner.generated.h"

//class AProp;
UCLASS()
class TAV_API APropSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APropSpawner();

	AActor* RotationTarget; 
	
	UFUNCTION()
		virtual void SetConstructionInvalid(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() 
		virtual void SetConstructionValid(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetConstructionValid();
	void SetConstructionInvalid();

	UPROPERTY()
		UDecalComponent* ArrowDecal;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
			UCapsuleComponent* CollisionCapsule; 
	
	UFUNCTION(BlueprintCallable)
		bool GetConstructionValid() { return bConstructionValid;  }
	

	UFUNCTION(BlueprintCallable)
	void SetProp(TSubclassOf<class AProp> inPropClass);

	UPROPERTY()
		TSubclassOf<class AProp> CurrentPropClass;

	UFUNCTION(BlueprintCallable)
		TSubclassOf<class AProp> GetCurrentProp() { return CurrentPropClass;  }

	UPROPERTY(BlueprintReadOnly)
		bool bConstructionValid; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UMaterialInterface* Mat_Invalid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UMaterialInterface* Mat_Valid;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY()
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(BlueprintReadOnly)
		bool bSpawnValid; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
