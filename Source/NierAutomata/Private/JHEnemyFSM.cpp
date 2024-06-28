// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemyFSM.h"
#include "JHBombSkill.h"
#include "JHMissileSkill.h"
#include "JHEnemy.h"
#include "JHBossSkillManager.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
UJHEnemyFSM::UJHEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UJHEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	Me = Cast<AJHEnemy>(GetOwner());

	//BombSkill = Me->BombSkill;
	//MissileSkill = Me->MissileSkill;
	
}


// Called every frame
void UJHEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString LogMsg = UEnum::GetValueAsString(MState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, LogMsg);

	switch (MState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	default:
		break;
	}
}

void UJHEnemyFSM::IdleState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if (CurrentTime > IdleDelayTime)
	{
		CurrentTime = 0;
		if (!bIsMove)
		{
			MState = EEnemyState::Move;
		}
	}

}

void UJHEnemyFSM::MoveState()
{
	// todo : 이동 위치 지정 필요
	

	// 중앙으로 이동
	FVector CurrPos = Me->GetActorLocation();
	//FVector DestPos = FVector::Zero() + FVector::UpVector * CurrPos.Z;
	FVector DestPos = FVector::RightVector*2000 + FVector::UpVector * CurrPos.Z;
	FVector Dir = DestPos - CurrPos;

	if (Dir.Size() <= AttackRange) {
		Me->SetActorLocation(DestPos);
		bIsMove = true;
		MState = EEnemyState::Attack;
	}

	Dir.Normalize();

	// 이동
	Me->SetActorLocation(CurrPos + Dir * MoveSpeed * GetWorld()->DeltaTimeSeconds);
	// 회전
	UKismetMathLibrary::FindLookAtRotation(CurrPos, DestPos);
}

void UJHEnemyFSM::AttackState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if (CurrentTime > AttackTime) {

		CurrentTime = 0;
		

		MState = EEnemyState::Idle;
	}

	//BombSkill->OnAttack();
	//MissileSkill->OnAttack();

	SkillManager->OnAttack();

}

void UJHEnemyFSM::DamageState()
{
}

void UJHEnemyFSM::DieState()
{
}

