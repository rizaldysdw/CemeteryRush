// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonEnemy.h"

ASkeletonEnemy::ASkeletonEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASkeletonEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ASkeletonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ASkeletonEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.0f)
	{
		Health = 0.0f;
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}
