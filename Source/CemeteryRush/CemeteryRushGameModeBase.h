// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CemeteryRushGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CEMETERYRUSH_API ACemeteryRushGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACemeteryRushGameModeBase();

private:
	// Number of enemies killed by the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Progress", meta = (AllowPrivateAccess = "true"))
	int32 KilledEnemy;

public:
	void IncrementKilledEnemy();
};
