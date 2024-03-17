// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class CEMETERYRUSH_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

protected:
	// Blackboard Component for this enemy
	UPROPERTY(BlueprintReadWrite, Category = "Artificial Intelligence", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;

	// Behavior Tree Component for this enemy
	UPROPERTY(BlueprintReadWrite, Category = "Artificial Intelligence", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;


public:
	virtual void OnPossess(APawn* InPawn) override;

public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }

};
