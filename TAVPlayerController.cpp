// Copyright Epic Games, Inc. All Rights Reserved.

#include "TAVPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "TAVCharacter.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"

ATAVPlayerController::ATAVPlayerController()
{
	bBuildMode = false; 
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	PropTurningRate = 15.f;

	NPCManager = CreateDefaultSubobject<UNPCManager>(TEXT("NPC Manager"));
	DeltaZoom = 50.f;
	ZoomMin = 300.f;
	ZoomMax = 3500.f; 
}

void ATAVPlayerController::ZoomIn()
{
	if (ATAVCharacter* MyPawn = Cast<ATAVCharacter>(GetPawn()))
	{
		MyPawn->GetCameraBoom()->TargetArmLength += -DeltaZoom;
		if (MyPawn->GetCameraBoom()->TargetArmLength > ZoomMax) MyPawn->GetCameraBoom()->TargetArmLength = ZoomMax;
		if (MyPawn->GetCameraBoom()->TargetArmLength < ZoomMin) MyPawn->GetCameraBoom()->TargetArmLength = ZoomMin;
	}
}

void ATAVPlayerController::ZoomOut()
{
	if (ATAVCharacter* MyPawn = Cast<ATAVCharacter>(GetPawn()))
	{
		MyPawn->GetCameraBoom()->TargetArmLength += DeltaZoom;
		if (MyPawn->GetCameraBoom()->TargetArmLength > ZoomMax) MyPawn->GetCameraBoom()->TargetArmLength = ZoomMax;
		if (MyPawn->GetCameraBoom()->TargetArmLength < ZoomMin) MyPawn->GetCameraBoom()->TargetArmLength = ZoomMin;
	}
}

void ATAVPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (PropSpawner_BP)
	{
		PropSpawner = GetWorld()->SpawnActor<APropSpawner>(PropSpawner_BP);
	}
}

void ATAVPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}

	if(bBuildMode)
	{
		if (ATAVCharacter* MyPawn = Cast<ATAVCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				if (PropSpawner)
				{
					PropSpawner->SetActorLocation(MyPawn->GetCursorToWorld()->GetComponentLocation());
				}
			}
		}
	}
}

void ATAVPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ATAVPlayerController::LeftClickBranch);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &ATAVPlayerController::RightClickBranch);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ATAVPlayerController::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ATAVPlayerController::ZoomOut);

	InputComponent->BindAction("ShowSpawner_DEBUG", IE_Pressed, this, &ATAVPlayerController::ShowSpawner);

	InputComponent->BindAction("ShowLabels", IE_Pressed, this, &ATAVPlayerController::ShowLabels);
	InputComponent->BindAction("ShowLabels", IE_Released, this, &ATAVPlayerController::HideLabels);

	InputComponent->BindAction("RotatePropR", IE_Pressed, this, &ATAVPlayerController::RotatePropR);
	InputComponent->BindAction("RotatePropL", IE_Pressed, this, &ATAVPlayerController::RotatePropL);
	
	//InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATAVPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ATAVPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATAVPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ATAVPlayerController::MoveToTouchLocation);
}

void ATAVPlayerController::LeftClickBranch()
{
	if(!bBuildMode)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Calling OnSetDestinationPressed"));
		OnSetDestinationPressed(); 
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Calling Construct"));
		if (PropSpawner->bConstructionValid)
		{
			Construct();
			HideSpawner();
		}
	}
}

void ATAVPlayerController::RightClickBranch()
{
	if(bBuildMode)
	{
		HideSpawner();
		bBuildMode = false; 
	}
}

void ATAVPlayerController::Construct()
{
	if (ATAVCharacter* MyPawn = Cast<ATAVCharacter>(GetPawn()))
		{
			GetWorld()->SpawnActor<AProp>(PropSpawner->GetCurrentProp(), PropSpawner->GetActorTransform());
		}
}

void ATAVPlayerController::RotatePropR()
{
	if(PropSpawner && bBuildMode)
	{
		PropSpawner->AddActorWorldRotation(FRotator(0, PropTurningRate, 0));
		//PropSpawner->AddActorLocalRotation(FRotator(0, PropTurningRate, 0)); 
	}
}

void ATAVPlayerController::RotatePropL()
{
	if (PropSpawner && bBuildMode)
	{
		PropSpawner->AddActorWorldRotation(FRotator(0, -PropTurningRate, 0));
	}
}

void ATAVPlayerController::HideSpawner()
{
	bBuildMode = false;
	PropSpawner->SetActorHiddenInGame(true); 
}

void ATAVPlayerController::ShowSpawner()
{
	bBuildMode = true; 
	
	if (ATAVCharacter* MyPawn = Cast<ATAVCharacter>(GetPawn()))
	{
		if (MyPawn->GetCursorToWorld())
		{
			
			if (PropSpawner)
			{
				
				PropSpawner->SetActorLocation(MyPawn->GetCursorToWorld()->GetComponentLocation());
				PropSpawner->SetActorHiddenInGame(false);
			}
		}
	}
}

void ATAVPlayerController::ShowLabels()
{
	for(auto n : NPCManager->NPCSet)
	{
		n->ShowNameFloater();
	}
}

void ATAVPlayerController::HideLabels()
{
	for (auto n : NPCManager->NPCSet)
	{
		n->HideNameFloater();
	}
}


void ATAVPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (ATAVCharacter* MyPawn = Cast<ATAVCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ATAVPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ATAVPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ATAVPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ATAVPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
