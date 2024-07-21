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
		SkillBases.Add(ESkillType::Bomb, MyOwner->BombSkill);
		SkillBases.Add(ESkillType::Missile, MyOwner->MissileSkill);
		SkillBases.Add(ESkillType::LaserBeam, MyOwner->LaserBeamSkill);
		SkillBases.Add(ESkillType::SpiralMove, MyOwner->SpiralMoveSkill);

		if (SkillPattern.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("SkillPattern Null!"));
			return;
		}

		UpdatePattern();
	
	}
	bPlayAttack = true;
}


// Called every frame
void UJHBossSkillManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UJHBossSkillManager::OnInitialize()
{
	// 스킬 패턴이 없을 경우 종료
	if (SkillPattern.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("SkillPattern Null!"));
		return;
	}
	if (CurrSkillType != ESkillType::SpiralMove)
	{
		SkillBases[CurrSkillType]->OnInitialize();

		bPlayAttack = false;
		MyOwnerFsm->OnChangeAttackPlay(false);
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
	if (bPlayAttack)
	{
		bPlayAttack = false;
		MyOwnerFsm->OnChangeAttackPlay(false);

		SkillBases[CurrSkillType]->OnStartAttack();
	}

	if (!bDelay) {

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

	MyOwner->SetRotSpeed(SkillPattern[PatternIndex].RotateSpeed);
	
	bPlayAttack = true;
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

	for (auto SkillBase : SkillBases)
	{
		SkillBase.Value->SetCurrSkillLevel(Level);
	}
}

void UJHBossSkillManager::SetAttackPlay(bool bValue)
{
	bPlayAttack = bValue;
}

void UJHBossSkillManager::OnDelayNextAttack()
{
	// 3초 뒤에 다음 공격으로 업데이트
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UJHBossSkillManager::UpdatePattern, 3, false);
}

void UJHBossSkillManager::SetRotateType(ERotateType Type)
{
	CurrRotateType = Type;
}