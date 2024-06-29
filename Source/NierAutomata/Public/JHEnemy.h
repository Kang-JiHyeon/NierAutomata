// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JHAttackInterface.h"
#include "JHEnemy.generated.h"

UCLASS()
class NIERAUTOMATA_API AJHEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJHEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	// Boss
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* RootCapsuleComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	// FSM
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UJHEnemyFSM* Fsm;

	 //Manager
	UPROPERTY(EditAnywhere)
	class UJHBossSkillManager* BossSkillManager;
};
