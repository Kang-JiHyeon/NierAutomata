// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBossSkillManager.h"
#include "JHEnemy.h"
#include "JHEnemyFsm.h"
#include "JHBombSkill.h"
#include "JHMissileSkill.h"
#include "JHGuidedMissileSkill.h"
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
		SkillBases.Add(ESkillType::GuidedMissile, MyOwner->GuidedMissileSkill);
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
	// ��ų ������ ���� ��� ����
	if (SkillPattern.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("SkillPattern Null!"));
		return;
	}

	SkillBases[CurrSkillType]->OnInitialize();

	bPlayAttack = false;
	MyOwnerFsm->OnChangeAttackPlay(false);

}

void UJHBossSkillManager::OnAttack()
{
	// ��ų ������ ���� ��� ����
	if (SkillPattern.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("SkillPattern Null!"));	
		return;
	}

	// ���� �ִϸ��̼� 
	if (bPlayAttack)
	{
		bPlayAttack = false;
		MyOwnerFsm->OnChangeAttackPlay(false);

		SkillBases[CurrSkillType]->OnStartAttack();
	}

	if (!bDelay) {

		 // ȸ��
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

void UJHBossSkillManager::OnEnd()
{
	// ��ų ������ ���� ��� ����
	if (SkillPattern.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("SkillPattern Null!"));
		return;
	}

	SkillBases[CurrSkillType]->OnEndAttack();

	bPlayAttack = false;
	MyOwnerFsm->OnChangeAttackPlay(false);
}


void UJHBossSkillManager::UpdatePattern()
{
	// ���� ��ų �ε��� 
	int32 RandIndex = FMath::RandRange(0, SkillPattern.Num() - 1);
	PatternIndex = RandIndex != PatternIndex ? RandIndex : (RandIndex + 1) % SkillPattern.Num();

	// ��ų ���� �ʱ�ȭ
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
	// 3�� �ڿ� ���� �������� ������Ʈ
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UJHBossSkillManager::UpdatePattern, 3, false);
}

void UJHBossSkillManager::SetRotateType(ERotateType Type)
{
	CurrRotateType = Type;
}