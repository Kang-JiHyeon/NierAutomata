// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBossSkillManager.h"
#include "JHEnemy.h"
#include "JHEnemyFsm.h"
#include "JHBombSkill.h"
#include "JHMissileSkill.h"
#include "JHLaserBeamSkill.h"
#include "JHSpiralMoveSkill.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UJHBossSkillManager::UJHBossSkillManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UJHBossSkillManager::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = Cast<AJHEnemy>(GetOwner());

	if (MyOwner != nullptr)
	{
		// FSM
		MyOwnerFsm = MyOwner->Fsm;
		// Skill
		BombSkill = MyOwner->BombSkill;
		MissileSkill = MyOwner->MissileSkill;
		LaserBeamSkill = MyOwner->LaserBeamSkill;
		SpiralMoveSkill = MyOwner->SpiralMoveSkill;

		SkillBases.Add(BombSkill);
		SkillBases.Add(MissileSkill);
		SkillBases.Add(LaserBeamSkill);
		SkillBases.Add(SpiralMoveSkill);

		if (SkillPattern.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("SkillPattern Null!"));
			return;
		}

		UpdatePattern();
	
	}
	bAttackPlay = true;
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
	case ESkillType::SpiralMove:
		SpiralMoveSkill->OnInitialize();
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

	// 공격 애니메이션 
	if (bAttackPlay)
	{
		bAttackPlay = false;
		MyOwnerFsm->OnChangeAttackPlay(false);

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
		case ESkillType::SpiralMove:
			SpiralMoveSkill->OnAttack();
			break;
		default:
			break;
		}

	}

	// 스킬 발동 시간이 최대 시간보다 클 경우
	//if (CurrCastTime > MaxCastTime)
	//{
	//	// 지연 시간이 흐르다가 
	//	CurrDelayTime += GetWorld()->DeltaTimeSeconds;

	//	if (!bDelay) {
	//		OnInitialize();
	//		bDelay = true;

	//	}

	//	// 지연 시간이 지난 후
	//	if (CurrDelayTime > MaxDelayTime)
	//	{	
	//		// 공격 패턴 변경
	//		//PatternIndex = (++PatternIndex) % SkillPattern.Num();
	//		UpdatePattern();

	//		// Sound 재생
	//		UGameplayStatics::PlaySound2D(GetWorld(), MyOwner->AttackSound);

	//		// 애니메이션 초기화
	//		MyOwnerFsm->OnChangeAttackPlay(true);
	//		
	//		bDelay = false;
	//	}
	////}
	//else 
	//{
	//	CurrCastTime += GetWorld()->DeltaTimeSeconds;
	//}

	if (!bDelay) {
		// 공격
		//switch (CurrSkillType)
		//{
		//case ESkillType::Bomb:
		//	BombSkill->OnAttack();
		//	break;
		//case ESkillType::Missile:
		//	MissileSkill->OnAttack();
		//	break;
		//case ESkillType::LaserBeam:
		//	LaserBeamSkill->OnAttack();
		//	break;
		//case ESkillType::SpiralMove:
		//	SpiralMoveSkill->OnAttack();
		//	break;
		//default:
		//	break;
		//}

		 // 회전
		switch (CurrRotateType)
		{
		case ERotateType::SpinBody:
			MyOwner->RotateSpinBody();
			break;
		case ERotateType::SpinBottom:
			MyOwner->RotateSpinBottom();
			break;
		case ERotateType::LookAt:
			MyOwner->RotateLookAt();
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
	// 랜덤 스킬 인덱스 
	PatternIndex = FMath::RandRange(0, SkillPattern.Num() - 1);
	
	// 스킬 정보 초기화
	CurrSkillType = SkillPattern[PatternIndex].SkillType;
	CurrRotateType = SkillPattern[PatternIndex].RotateType;

	MaxCastTime = SkillPattern[PatternIndex].CastTime;
	MaxDelayTime = SkillPattern[PatternIndex].DelayTime;

	MyOwner->SetRotSpeed(SkillPattern[PatternIndex].RotateSpeed);
	
	// 지속, 대기 시간 초기화
	CurrCastTime = 0;
	CurrDelayTime = 0;

	bAttackPlay = true;
	MyOwnerFsm->OnChangeAttackPlay(true);


	UE_LOG(LogTemp, Warning, TEXT("Update Pattern :  %d"), PatternIndex);
}

void UJHBossSkillManager::OnToggleSkillLevel()
{
	if (SkillBases.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnToggleSkillLevel : SkillBases Null!!"));
		return;
	}

	bHardLevel = !bHardLevel;

	ESkillLevel Level = bHardLevel ? ESkillLevel::Hard : ESkillLevel::Easy;

	for (auto Skill : SkillBases)
	{
		Skill->SetCurrSkillLevel(Level);
	}
}

void UJHBossSkillManager::SetAttackPlay(bool bValue)
{
	bAttackPlay = bValue;
}

void UJHBossSkillManager::SetRotateType(ERotateType Type)
{
	CurrRotateType = Type;
}