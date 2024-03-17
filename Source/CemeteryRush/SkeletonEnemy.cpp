// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonEnemy.h"
#include "Components/SphereComponent.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "CemeteryRushGameModeBase.h"

ASkeletonEnemy::ASkeletonEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Combat Sphere Collision"));
	CombatSphereCollision->SetupAttachment(GetRootComponent());

	LeftHandSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Left Hand Collision"));
	LeftHandSphereCollision->SetupAttachment(GetMesh(), FName("hand_l"));

	RightHandSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Right Hand Collision"));
	RightHandSphereCollision->SetupAttachment(GetMesh(), FName("hand_r"));

	MaxHealth = 150.f;
	BaseDamage = 10.f;
	bCanAttack = true;
	bInAttackRange = false;
}

void ASkeletonEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Set maximum health as current health
	Health = MaxHealth;

	// Setup collisions
	CombatSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkeletonEnemy::OnCombatSphereBeginOverlap);
	CombatSphereCollision->OnComponentEndOverlap.AddDynamic(this, &ASkeletonEnemy::OnCombatSphereEndOverlap);

	LeftHandSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkeletonEnemy::OnLeftHandSphereBeginOverlap);
	RightHandSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkeletonEnemy::OnRightHandSphereBeginOverlap);
	
	LeftHandSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandSphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftHandSphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftHandSphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
	RightHandSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandSphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightHandSphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightHandSphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// Get AI Controller
	EnemyAIController = Cast<AEnemyAIController>(GetController());

	if (EnemyAIController)
	{
		auto PlayerClass = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
		auto Player = Cast<APlayerCharacter>(PlayerClass);

		EnemyAIController->GetBlackboardComponent()->SetValueAsObject("Player", Player);

		EnemyAIController->RunBehaviorTree(BehaviorTree);
	}
}

void ASkeletonEnemy::PerformMeleeAttack(FName AttackSectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && bCanAttack)
	{
		

		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(AttackSectionName, AttackMontage);
	}

	bCanAttack = false;
}

FName ASkeletonEnemy::GetAttackAnimName()
{
	int32 AttackSection{ FMath::RandRange(1, 2)};

	switch (AttackSection)
	{
		case 1:
			return FName("RightAttack");
			break;
		case 2:
			return FName("LeftAttack");
			break;
	}

	return FName();
}

void ASkeletonEnemy::ResetCanAttack()
{
	bCanAttack = true;
}

void ASkeletonEnemy::Die()
{
	auto GameMode = Cast<ACemeteryRushGameModeBase>(UGameplayStatics::GetGameMode);
	GameMode->IncrementKilledEnemy();
	
	Destroy();
}

void ASkeletonEnemy::OnCombatSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	auto Character = Cast<APlayerCharacter>(OtherActor);
	
	if (Character)
	{
		bInAttackRange = true;

		if (EnemyAIController)
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("InAttackRange"), true);
		}
	}
}

void ASkeletonEnemy::OnCombatSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;

	auto Character = Cast<APlayerCharacter>(OtherActor);
	if (Character)
	{
		bInAttackRange = false;

		if (EnemyAIController)
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("InAttackRange"), false);
		}
	}
}

void ASkeletonEnemy::OnLeftHandSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	auto PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (PlayerCharacter)
	{
		UGameplayStatics::ApplyDamage(PlayerCharacter, BaseDamage, EnemyAIController, this, UDamageType::StaticClass());
	}
}

void ASkeletonEnemy::OnRightHandSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	auto PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (PlayerCharacter)
	{
		UGameplayStatics::ApplyDamage(PlayerCharacter, BaseDamage, EnemyAIController, this, UDamageType::StaticClass());
	}
}

void ASkeletonEnemy::EnableLeftHandSphereCollision()
{
	LeftHandSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASkeletonEnemy::DisableLeftHandSphereCollision()
{
	LeftHandSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASkeletonEnemy::EnableRightHandSphereCollision()
{
	RightHandSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASkeletonEnemy::DisableRightHandSphereCollision()
{
	RightHandSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
		Die();
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}
