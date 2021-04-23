// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NPCManager.h"
#include "Props/PropChair.h"
#include "Props/PropTable.h"
#include "PropSpawner.h"
#include "TAVPlayerController.generated.h"

class ANPC; 

UCLASS()
class ATAVPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATAVPlayerController();

	UPROPERTY()
		APropTable* CurrentTable; 

	UPROPERTY()
		UNPCManager* NPCManager;
	
protected:

	//ZOOMING

	UFUNCTION()
	void ZoomIn();

	UFUNCTION()
	void ZoomOut();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float DeltaZoom;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ZoomMin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ZoomMax;
	
	UPROPERTY()
		APropSpawner* PropSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
		TSubclassOf<APropSpawner> PropSpawner_BP; 

	UFUNCTION()
		void Construct();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float PropTurningRate;
	
	UFUNCTION()
		void RotatePropR();

	UFUNCTION()
		void RotatePropL();


	UFUNCTION()
	void HideSpawner();


	UFUNCTION(BlueprintCallable, Category = "Spawner")
		APropSpawner* GetSpawner() { return PropSpawner; }
	
	UFUNCTION(BlueprintCallable, Category = "Spawner")
		void ShowSpawner();

	UFUNCTION(BlueprintCallable)
		void ShowLabels();

	UFUNCTION(BlueprintCallable)
		void HideLabels();

	UPROPERTY()
		bool bBuildMode; 

	UFUNCTION()
	void LeftClickBranch();

	UFUNCTION()
	void RightClickBranch();
	
	virtual void BeginPlay() override; 
	
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};


