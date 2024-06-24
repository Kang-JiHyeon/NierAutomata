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
	// 에너미 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	EEnemyState mState = EEnemyState::Idle;

	float idleDelayTime = 2;

	// 타겟
	
	// 이동 속도
	float moveSpeed = 1.f;
	// 체력
	int32 maxHp = 100;
private:
	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();
};
