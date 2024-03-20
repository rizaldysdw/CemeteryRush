// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class CEMETERYRUSH_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

private:
	// Parent mesh of the door. Add door children manually in Blueprint Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DoorParentMesh;

	// Target killed enemies to allow this door to open
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties", meta = (AllowPrivateAccess = "true"))
	int32 TargetKilledEnemies;

	// True when door should open
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door Properties", meta = (AllowPrivateAccess = "true"))
	bool bShouldOpen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Mode Base", meta = (AllowPrivateAccess = "true"))
	class ACemeteryRushGameModeBase* GameModeBase;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called in Begin Play to set GameModeBase variable
	void SetGameModeBase();

	// Return true when door should open
	UFUNCTION(BlueprintPure)
	bool DoorShouldOpen();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
