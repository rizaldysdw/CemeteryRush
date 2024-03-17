// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter() :
	// Camera
	BaseTurnRate(60.f),
	BaseLookUpRate(60.f),
	MouseTurnRate(1.f),
	MouseLookUpRate(1.f),
	
	// Combat
	bMeleeAttackPressed(false),
	bCanAttack(true),
	AttackCount(0)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a Camera Boom
	// Pulls in towards the character if there is a collision
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	// Attach camera to the end of CameraBoom
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Rotation rate of the character
	GetCharacterMovement()->RotationRate = FRotator(0, 540.0f, 0);

	// Character jump height
	GetCharacterMovement()->JumpZVelocity = 600.0f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0)
	{
		// Find out which way is forward
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };

		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0)
	{
		// Find out which way is right
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };

		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * MouseTurnRate);
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * MouseLookUpRate);
}

void APlayerCharacter::TurnRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::MeleeAttackPressed()
{
	bMeleeAttackPressed = true;
	PerformMeleeAttack();
}

void APlayerCharacter::MeleeAttackReleased()
{
	bMeleeAttackPressed = false;
}

void APlayerCharacter::PerformMeleeAttack()
{
	if (!bCanAttack) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (bMeleeAttackPressed)
	{
		bCanAttack = false;
		AttackCount++;

		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(GetAttackAnimName(), AttackMontage);
	}
}

void APlayerCharacter::ResetCanAttack()
{
	bCanAttack = true;
}

FName APlayerCharacter::GetAttackAnimName()
{
	switch (AttackCount)
	{
	case 1:
		return FName("AttackA");
		break;

	case 2:
		return FName("AttackB");
		break;

	case 3:
		return FName("AttackC");
	}

	return FName();
}

void APlayerCharacter::ResetAttackCount()
{
	AttackCount = 0;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("MeleeAttack", IE_Pressed, this, & APlayerCharacter::MeleeAttackPressed);
	PlayerInputComponent->BindAction("MeleeAttack", IE_Released, this, &APlayerCharacter::MeleeAttackReleased);
}

