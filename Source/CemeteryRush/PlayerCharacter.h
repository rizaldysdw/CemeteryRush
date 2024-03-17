// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class CEMETERYRUSH_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

private:
	// Camera Boom positioning behind character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Camera that follows the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Base turn rate, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	// Base look up/down rate, in deg/sec. Other scaling may affect final look up rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	// Mouse turn rate, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseTurnRate;

	// Mouse look up/down rate, in deg/sec. Other scaling may affect final look up rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseLookUpRate;

	// Anim Montage containing attack animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	// Box Collision to deal damage to other Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CombatBoxCollision;

	// True when Melee Attack input is pressed, false when released
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bMeleeAttackPressed;

	// True when Player can perform attack
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bCanAttack;

	// Integer to determine which attack animation should be played
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int AttackCount;

	// Damage to apply to other actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the character receive vertical input
	void MoveForward(float Value);

	// Called when the character receive horizontal input
	void MoveRight(float Value);

	/* Rotate controller based on Mouse X input
	@param Value - The input value from mouse movement */
	void Turn(float Value);

	/* Rotate controller based on Mouse Y input
	@param Value - The input value from mouse movement */
	void LookUp(float Value);

	/* Called via input to turn at a given rate.
	@param Rate - This is a normalized rate, i.e. 1.0 means 100% of desired turn rate */
	void TurnRate(float Rate);

	/* Called via input to look up/down at a given rate.
	@param Rate - This is a normalized rate, i.e. 1.0 means 100 % of desired rate */
	void LookUpRate(float Rate);

	// Set bMeleeAttack to true
	void MeleeAttackPressed();

	// Set bMeleeAttack to false
	void MeleeAttackReleased();

	// Call this function to perform melee attack behavior
	void PerformMeleeAttack();

	// Call this function in AnimBlueprint Anim Notify
	UFUNCTION(BlueprintCallable)
	void ResetCanAttack();

	// Return FName of the attack animation in Attack Anim Montage
	FName GetAttackAnimName();

	// Reset Attack Count integer
	UFUNCTION(BlueprintCallable)
	void ResetAttackCount();

	// Called to enable combat collision
	UFUNCTION(BlueprintCallable)
	void EnableCombatCollision();

	// Called to disable combat collision
	UFUNCTION(BlueprintCallable)
	void DisableCombatCollision();

	void OnCombatBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
