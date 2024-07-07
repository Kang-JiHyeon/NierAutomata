// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JHBossSkillManager.generated.h"



UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Bomb,
	Missile,
	LaserBeam,
	SpiralMove,
};

UENUM(BlueprintType)
enum class ERotateType : uint8
{
	None,
	SpinBody,
	SpinBottom,
	LookAt,
	Target
};

USTRUCT(Atomic)
struct FSkillProperty
{
	GENERATED_USTRUCT_BODY()
public :
	UPROPERTY(EditAnywhere)
	ESkillType SkillType;
	UPROPERTY(EditAnywhere)
	float CastTime;
	UPROPERTY(EditAnywhere)
	float DelayTime;
	UPROPERTY(EditAnywhere)
	ERotateType RotateType;
	UPROPERTY(EditAnywhere)
	float RotateSpeed;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHBossSkillManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJHBossSkillManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType CurrSkillType = ESkillType::Bomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERotateType CurrRotateType = ERotateType::None;

	UPROPERTY(VisibleAnywhere)
	class AJHEnemy* MyBoss;

	UPROPERTY(VisibleAnywhere)
	class UJHBombSkill* BombSkill;

	UPROPERTY(VisibleAnywhere)
	class UJHMissileSkill* MissileSkill;

	UPROPERTY(VisibleAnywhere)
	class UJHLaserBeamSkill* LaserBeamSkill;

	UPROPERTY(VisibleAnywhere)
	class UJHSpiralMoveSkill* SpiralMoveSkill;

	UPROPERTY(EditAnywhere)
	TArray<FSkillProperty> SkillPattern;


	float MaxCastTime = 0;
	float CurrCastTime = 0;
	
	float MaxDelayTime = 0;
	float CurrDelayTime = 0;

	int32 PatternIndex = 0;
	
	bool bAttacking = false;
	bool bDelay = false;

public:
	void OnInitialize();
	void OnAttack();

private:
	void UpdatePattern();
	void SetRotateType(ERotateType Type);
};
