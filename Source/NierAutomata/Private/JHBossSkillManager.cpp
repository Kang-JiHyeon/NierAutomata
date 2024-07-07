// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBossSkillManager.h"
#include "JHEnemy.h"
#include "JHBombSkill.h"
#include "JHMissileSkill.h"
#include "JHLaserBeamSkill.h"

// Sets default values for this component's properties
UJHBossSkillManager::UJHBossSkillManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UJHBossSkillManager::BeginPlay()
{
	Super::BeginPlay();

	MyBoss = Cast<AJHEnemy>(GetOwner());

	if (MyBoss != nullptr)
	{
		if (SkillPattern.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("SkillPattern Null!"));
			return;
		}

		UpdatePattern();
	}

}


// Called every frame
void UJHBossSkillManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UJHBossSkillManager::OnInitialize()
{
	
	switch (CurrSkillType)
	{
	case ESkillType::Bomb:
		BombSkill->OnInitialize();
		break;
	case ESkillType::Missile:
		MissileSkill->OnInitialize();
		break;
	case ESkillType::LaserBeam:
		LaserBeamSkill->OnInitialize();
		break;
	default:
		break;
	}
}


void UJHBossSkillManager::OnAttack()
{
	// 스킬 패턴이 없을 경우 종료
	if (SkillPattern.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("SkillPattern Null!"));	
		return;
	}

	if (CurrCastTime > MaxCastTime)
	{
		CurrDelayTime += GetWorld()->DeltaTimeSeconds;

		if (!bDelay) {
			OnInitialize();
			bDelay = true;
		}

		if (CurrDelayTime > MaxDelayTime)
		{	
			PatternIndex = (++PatternIndex) % SkillPattern.Num();
			
			UpdatePattern();

			bDelay = false;
		}
	}
	else 
	{
		CurrCastTime += GetWorld()->DeltaTimeSeconds;
	}

	if (!bDelay) {
		// 공격
		switch (CurrSkillType)
		{
		case ESkillType::Bomb:
			BombSkill->OnAttack();
			break;
		case ESkillType::Missile:
			MissileSkill->OnAttack();
			break;
		case ESkillType::LaserBeam:
			LaserBeamSkill->OnAttack();
			break;
		default:
			break;
		}

		 // 회전
		switch (CurrRotateType)
		{
		case ERotateType::SpinBody:
			MyBoss->RotateSpinBody();
			break;
		case ERotateType::SpinBottom:
			MyBoss->RotateSpinBottom();
			break;
		case ERotateType::LookAt:
			MyBoss->RotateLookAt();
			break;
		case ERotateType::Target:
			//MyBoss->RotateTarget();
			break;
		default:
			break;
		}
	}

}

void UJHBossSkillManager::UpdatePattern()
{
	CurrSkillType = SkillPattern[PatternIndex].SkillType;
	CurrRotateType = SkillPattern[PatternIndex].RotateType;

	MaxCastTime = SkillPattern[PatternIndex].CastTime;
	MaxDelayTime = SkillPattern[PatternIndex].DelayTime;

	CurrCastTime = 0;
	CurrDelayTime = 0;

	MyBoss->SetRotSpeed(SkillPattern[PatternIndex].RotateSpeed);
}