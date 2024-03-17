// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "SkeletonEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CEMETERYRUSH_API ASkeletonEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	ASkeletonEnemy();

private:
	// Sphere Collision for when this actor can attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CombatSphereCollision;

	// Sphere Collision for left hand of this Pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USphereComponent* LeftHandSphereCollision;

	// Sphere Collision for right hand of this Pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USphereComponent* RightHandSphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Call this function to perform melee attack behavior
	void PerformMeleeAttack();

	// Return FName of the attack animation in Attack Anim Montage
	FName GetAttackAnimName();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to receive damage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
