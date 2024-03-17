// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
}

void UPlayerAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}

	if (PlayerCharacter)
	{
		// Get the lateral speed of the character from velocity
		FVector Velocity{ PlayerCharacter->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		// Check if the character is in air
		bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();

		// Check if the character is moving
		if (PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
		{
			bIsMoving = true;
		}
		else
		{
			bIsMoving = false;
		}
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
}
