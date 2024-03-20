// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "CemeteryRushGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor() :
	TargetKilledEnemies(0),
	bShouldOpen(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Static Mesh Component for the door parent and set it as Root Component
	DoorParentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Parent Mesh"));
	SetRootComponent(DoorParentMesh);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	SetGameModeBase();
}

void ADoor::SetGameModeBase()
{
	if (GameModeBase == nullptr)
	{
		AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
		GameModeBase = Cast<ACemeteryRushGameModeBase>(CurrentGameMode);
	}
	else
	{
		return;
	}
}

bool ADoor::DoorShouldOpen()
{
	if (TargetKilledEnemies == GameModeBase->GetKilledEnemy())
	{
		bShouldOpen = true;
	}
	else
	{
		bShouldOpen = false;
	}

	return bShouldOpen;
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoorShouldOpen();
}

