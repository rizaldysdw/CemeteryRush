// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class CEMETERYRUSH_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Maximum health of this Pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	
	// Current health of this Pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health;

	// Base damage of this Pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	// True when this Pawn can perform attack to other actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bCanAttack;

	// True when other actor is in Attack Range of this Pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bInAttackRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Artificial Intelligence", meta = (AllowPrivateAccess = "true"))
	class AEnemyAIController* EnemyAIController;

	// Behavior Tree for the AI Enemy
	UPROPERTY(EditAnywhere, Category = "Artificial Intelligence", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
};
