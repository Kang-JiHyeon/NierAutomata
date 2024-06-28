// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBossSkillManager.h"
#include "JHEnemy.h"
#include "JHMissileSkill.h"
#include "JHBombSkill.h"

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

	CurrSkillType = SkillPattern[PatternIndex].SkillType;
	
	MaxCastTime = SkillPattern[PatternIndex].CastTime;
	MaxDelayTime = SkillPattern[PatternIndex].DelayTime;
	CurrCastTime = 0;
	CurrCastTime = 0;

}


// Called every frame
void UJHBossSkillManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

		if(!bDelay)
			bDelay = true;

		if (CurrDelayTime > MaxDelayTime)
		{	
			OnInitialize();
			
			PatternIndex = (++PatternIndex) % SkillPattern.Num();

			CurrSkillType = SkillPattern[PatternIndex].SkillType;
			MaxCastTime = SkillPattern[PatternIndex].CastTime;
			MaxDelayTime = SkillPattern[PatternIndex].DelayTime;

			CurrCastTime = 0;
			CurrDelayTime = 0;

			bDelay = false;
		}
	}
	else {
		CurrCastTime += GetWorld()->DeltaTimeSeconds;
	}

	if (!bDelay) {
		switch (CurrSkillType)
		{
		case ESkillType::Bomb:
			BombSkill->OnAttack();
			break;
		case ESkillType::Missile:
			MissileSkill->OnAttack();
			break;
		case ESkillType::LaserBeam:
			break;
		default:
			break;
		}
	}
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
		break;
	default:
		break;
	}
}

