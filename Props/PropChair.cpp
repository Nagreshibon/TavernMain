// Fill out your copyright notice in the Description page of Project Settings.


#include "PropChair.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TAV/TAVPlayerController.h"

APropChair::APropChair()
{
	SitMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sit Marker"));
	SitMarker->SetupAttachment(RootComponent);
	SitMarker->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	
	bStartInvalid = true; 
}

bool APropChair::PostOverlap(UPrimitiveComponent* Comp, AActor* OtherActor)
{
	if(APropTable* MyTable = Cast<APropTable>(OtherActor))
	{
		if (MyTable->ChairArray.Num() >= MyTable->ChairSlots) return false; 

		UE_LOG(LogTemp, Warning, TEXT("Snapping to table"));
		
		FVector TableLocation = MyTable->GetActorLocation();
		FVector MyLocation = Comp->GetOwner()->GetActorLocation(); 
		FRotator Rotation = (TableLocation - MyLocation).ToOrientationRotator();

		Comp->GetOwner()->SetActorRotation(Rotation);

		if(ATAVPlayerController* MyController = Cast<ATAVPlayerController>(UGameplayStatics::GetPlayerController(MyTable->GetWorld(),0)))
		{
			MyController->CurrentTable = MyTable;

			if (APropSpawner* MySpawner = Cast<APropSpawner>(Comp->GetOwner()))
			{
				MySpawner->RotationTarget = MyTable; 
				MySpawner->ArrowDecal->SetHiddenInGame(false);
				MySpawner->SetConstructionValid(); 
			}
			
		}
		
		return false; 
	}

	UE_LOG(LogTemp, Warning, TEXT("NOT TABLE"));
	
	return true; 
}

bool APropChair::PostEndOverlap(UPrimitiveComponent* Comp, AActor* OtherActor)
{
	if (APropTable* MyTable = Cast<APropTable>(OtherActor))
	{
		if (ATAVPlayerController* MyController = Cast<ATAVPlayerController>(UGameplayStatics::GetPlayerController(MyTable->GetWorld(), 0)))
		{
			MyController->CurrentTable = nullptr;
			if(APropSpawner* MySpawner = Cast<APropSpawner>(Comp->GetOwner()))
			{
				MySpawner->SetConstructionInvalid();
				MySpawner->ArrowDecal->SetHiddenInGame(true);
				MySpawner->RotationTarget = nullptr; 
			}
			
		}
		return false; 
	}

	if (APropSpawner* MySpawner = Cast<APropSpawner>(Comp->GetOwner()))
	{
		MySpawner->SetConstructionInvalid();
		MySpawner->RotationTarget = nullptr;
	}
	
	return false;
}

void APropChair::BeginPlay()
{
	Super::BeginPlay();

	if (ATAVPlayerController* MyController = Cast<ATAVPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if(MyController->CurrentTable)
		{
			OwningTable = MyController->CurrentTable;
			MyController->CurrentTable->ChairArray.Add(this); 
		}
	}
}
