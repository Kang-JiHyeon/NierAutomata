// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JHEnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die
};

UENUM(BlueprintType)
enum class EAttackSkillState : uint8
{
	Bomb,
	Laser,
	Ring
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJHEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// ���ʹ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSMComponent)
	EEnemyState MState = EEnemyState::Idle;

	// ���
	UPROPERTY(EditAnywhere)
	float IdleDelayTime = 2;
	float CurrentTime = 0;

	// �̵�
	UPROPERTY(VisibleAnywhere)
	class AActor* Target;

	UPROPERTY()
	class AJHEnemy* Me;
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 500;
	UPROPERTY(EditAnywhere)
	float AttackRange = 10;

	UPROPERTY()
	bool bIsMove;

	// ����
	UPROPERTY()
	class UJHBombSkill* BombSkill;
	
	UPROPERTY()
	class UJHMissileSkill* MessileSkill;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackSkillState SkillState = EAttackSkillState::Bomb;

	UPROPERTY(EditAnywhere)
	float AttackTime = 5;


	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();
};