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
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	// FSM
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UJHEnemyFSM* Fsm;

	/* Skill */
	 //Manager
	UPROPERTY(EditAnywhere)
	class UJHBossSkillManager* BossSkillManager;

	UPROPERTY()
	TArray<class IJHAttackInterface*> AttackSkills;

	// Bomb
	UPROPERTY(EditAnywhere)
	class UJHBombSkill* BombSkill;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AJHBomb> BombFactory;

	UPROPERTY(EditAnywhere)
	TArray<class USceneComponent*> FirePositions;

	UPROPERTY(EditAnywhere)
	int BombCount = 10;


	// Missile
	UPROPERTY(EditAnywhere)
	class UJHMissileSkill* MissileSkill;

	UPROPERTY()
	TSubclassOf<class AJHMissile> MissileFactory;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* MissileArrow;

};
