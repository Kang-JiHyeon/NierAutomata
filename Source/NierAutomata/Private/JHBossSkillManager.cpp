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
	// ��ų ������ ���� ��� ����
	if (SkillPattern.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("SkillPattern Null!"));	
		return;
	}

	// ���� �ִϸ��̼� 
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

	// ��ų �ߵ� �ð��� �ִ� �ð����� Ŭ ���
	//if (CurrCastTime > MaxCastTime)
	//{
	//	// ���� �ð��� �帣�ٰ� 
	//	CurrDelayTime += GetWorld()->DeltaTimeSeconds;

	//	if (!bDelay) {
	//		OnInitialize();
	//		bDelay = true;

	//	}

	//	// ���� �ð��� ���� ��
	//	if (CurrDelayTime > MaxDelayTime)
	//	{	
	//		// ���� ���� ����
	//		//PatternIndex = (++PatternIndex) % SkillPattern.Num();
	//		UpdatePattern();

	//		// Sound ���
	//		UGameplayStatics::PlaySound2D(GetWorld(), MyOwner->AttackSound);

	//		// �ִϸ��̼� �ʱ�ȭ
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
		// ����
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

void UJHBossSkillManager::UpdatePattern()
{
	// ���� ��ų �ε��� 
	PatternIndex = FMath::RandRange(0, SkillPattern.Num() - 1);
	
	// ��ų ���� �ʱ�ȭ
	CurrSkillType = SkillPattern[PatternIndex].SkillType;
	CurrRotateType = SkillPattern[PatternIndex].RotateType;

	MaxCastTime = SkillPattern[PatternIndex].CastTime;
	MaxDelayTime = SkillPattern[PatternIndex].DelayTime;

	MyOwner->SetRotSpeed(SkillPattern[PatternIndex].RotateSpeed);
	
	// ����, ��� �ð� �ʱ�ȭ
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