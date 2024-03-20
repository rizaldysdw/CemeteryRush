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

	/*If true, Character Movement will be auto activated at BeginPlay.
	If false, Character Movement activation should be set to true manually */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bAutoActive;

	// Target killed enemies to allow this character to move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 TargetKilledEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Mode Base", meta = (AllowPrivateAccess = "true"))
	class ACemeteryRushGameModeBase* GameModeBase;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Call this function to perform melee attack behavior
	UFUNCTION(BlueprintCallable)
	void PerformMeleeAttack(FName AttackSectionName);

	// Return FName of the attack animation in Attack Anim Montage
	UFUNCTION(BlueprintPure)
	FName GetAttackAnimName();

	// Called to reset bCanAttack to true
	UFUNCTION(BlueprintCallable)
	void ResetCanAttack();

	// Called when this Pawn die
	UFUNCTION(BlueprintCallable)
	void Die();

	// Called in Begin Play to set GameModeBase variable
	void SetGameModeBase();

	// Called in Tick to activate Character Movement Component based on certain conditions
	void ActivateCharacterMovementAtRuntime();

	// Called when an Actor begins overlapping with Combat Sphere Collision
	UFUNCTION()
	void OnCombatSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// Called when an Actor stops overlapping with Combat Sphere Collision
	UFUNCTION()
	void OnCombatSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called when an Actor begins overlapping with Left Hand Sphere Collision
	UFUNCTION()
	void OnLeftHandSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// Called when an Actor begins overlapping with Right Hand Sphere Collision
	UFUNCTION()
	void OnRightHandSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// Called to enable Left Hand Sphere Collision
	UFUNCTION(BlueprintCallable)
	void EnableLeftHandSphereCollision();

	// Called to disable Left Hand Sphere Collision
	UFUNCTION(BlueprintCallable)
	void DisableLeftHandSphereCollision();

	// Called to enable Right Hand Sphere Collision
	UFUNCTION(BlueprintCallable)
	void EnableRightHandSphereCollision();

	// Called to disable Right Hand Sphere Collision
	UFUNCTION(BlueprintCallable)
	void DisableRightHandSphereCollision();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to receive damage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable)
	void ActivateCharacterMovement();
};
