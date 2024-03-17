// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonEnemy.h"
#include "Components/SphereComponent.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

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

void ASkeletonEnemy::PerformMeleeAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (bCanAttack)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(GetAttackAnimName(), AttackMontage);
	}
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
