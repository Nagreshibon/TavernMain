// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "Prop.generated.h"

UCLASS(Blueprintable)
class TAV_API AProp : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
		FGameplayTagContainer GameplayTags; 
	
	UFUNCTION(BlueprintCallable)
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; return; };
	
	UStaticMeshComponent* GetMesh() { return StaticMesh; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bStartInvalid; 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UDecalComponent* ArrowDecal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UCapsuleComponent* CollisionCapsule;

	UFUNCTION()
		virtual bool PostOverlap(UPrimitiveComponent* Comp, AActor* OtherActor);

	UFUNCTION()
		virtual bool PostEndOverlap(UPrimitiveComponent* Comp, AActor* OtherActor);

	FORCEINLINE float GetPrice() { return Price; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ConstructCurrent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ConstructTotal;
	
	UFUNCTION(BlueprintCallable)
		void Construct(float inAmount);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UMaterialInterface* UnderConstructionMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UMaterialInterface* OriginalMaterial;

	UPROPERTY()
		float OriginalMaterialExponent;

	UPROPERTY()
		UMaterialInstanceDynamic* ConstructionMaterialInstance; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Weight; 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FTransform Anchor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CurrentDurability;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MaxDurability;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Price;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	USceneComponent* DefaultSceneRoot;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
