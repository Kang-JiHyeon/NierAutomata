// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JHEnemyFSM.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateDynamicOneParamSignature, int32, InValue);

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

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "OnChangedHp"))
	FDelegateDynamicOneParamSignature OnChangedHp;

public:
    UPROPERTY(VisibleAnywhere)
    class AJHEnemy* MyOwner;

	// 에너미 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState EnemyState = EEnemyState::Idle;

	// Anim
	UPROPERTY(EditAnywhere)
	class UJHBossAnimInstance* BossAnim;

	// Idle
	UPROPERTY(EditAnywhere)
	float IdleDelayTime = 2;
	float CurrentTime = 0;

	// Move
	UPROPERTY(VisibleAnywhere)
	class AActor* Target;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 500;
	UPROPERTY(EditAnywhere)
	float AttackRange = 10;

	bool bIsMove;

	// Attack
	UPROPERTY(EditAnywhere)
	class UJHBossSkillManager* SkillManager;

	UPROPERTY(EditAnywhere)
	float AttackTime = 20;


	// Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp = 10;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Hp;
	UPROPERTY(EditAnywhere)
	float DamageRate = 0.5f;
	UPROPERTY(EditAnywhere)
	float DamageTime = 5;
	bool bIsPlayDamageAnim;
	bool bActiveDamageParticle;

	// todo : 애니메이션 및 이펙트로 변경
	UPROPERTY(EditAnywhere)
	UMaterialInterface* DamageMaterial;
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* PSFire;

	UPROPERTY()
	class UParticleSystemComponent* PSDamageComp;
	UPROPERTY()
	FTimerHandle TimerHandle;

	// Die
	UPROPERTY(EditAnywhere)
	float DieTime = 5;
	UPROPERTY(EditAnywhere)
	float DieMoveSpeed = 300;
	

public:
	void OnChangeAnimState();
	void OnChangeAttackPlay(bool bValue);

	void OnDamageProcess(int32 Damage);
private:
	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();
};