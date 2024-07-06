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

	MyOwner = Cast<AJHEnemy>(GetOwner());
	
	Hp = MaxHp;

	DefaultMaterial = MyOwner->GetBodyMaterial();
}


// Called every frame
void UJHEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString LogMsg = UEnum::GetValueAsString(EnemyState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, LogMsg);

	switch (EnemyState)
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

/// <summary>
/// Idle ������ �� ȣ��Ǵ� �Լ�
/// - ���� �ð����� ����ϴٰ� Move ���·� ��ȯ
/// </summary>
void UJHEnemyFSM::IdleState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if (CurrentTime > IdleDelayTime)
	{
		CurrentTime = 0;
		EnemyState = EEnemyState::Move;
	}

}

/// <summary>
/// Move ������ �� ȣ��Ǵ� �Լ�
/// - Ÿ�� ��ġ�� �̵��ϰ� Attack ���·� ��ȯ
/// - todo : Ÿ�� ��ġ ���� �ʿ�!!
/// </summary>
void UJHEnemyFSM::MoveState()
{
	// todo : �̵� ��ġ ������ �ʿ�
	
	// �߾����� �̵�
	FVector CurrPos = MyOwner->GetActorLocation();
	FVector DestPos = FVector::Zero() + FVector::UpVector * CurrPos.Z;
	FVector Dir = DestPos - CurrPos;

	if (Dir.Size() <= AttackRange) {
		MyOwner->SetActorLocation(DestPos);
		bIsMove = true;

		EnemyState = EEnemyState::Attack;
	}

	Dir.Normalize();

	// �̵�
	MyOwner->SetActorLocation(CurrPos + Dir * MoveSpeed * GetWorld()->DeltaTimeSeconds);
	// ȸ��
	UKismetMathLibrary::FindLookAtRotation(CurrPos, DestPos);
}

/// <summary>
/// Attack ������ �� ȣ��Ǵ� �Լ�
/// - ���� �ð����� �����ϴٰ� ��� ���·� ��ȯ
/// </summary>
void UJHEnemyFSM::AttackState()
{
	//CurrentTime += GetWorld()->DeltaTimeSeconds;

	//if (CurrentTime > AttackTime) {

	//	CurrentTime = 0;
	//	EnemyState = EEnemyState::Idle;
	//}

	SkillManager->OnAttack();
}

/// <summary>
/// Damage ������ �� ȣ��Ǵ� �Լ�
/// - ���� �ð����� Damage ���¸� �����Ѵ�.
/// </summary>
void UJHEnemyFSM::DamageState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if (CurrentTime > DamageTime)
	{
		MyOwner->SetBodyMaterial(DefaultMaterial);
		CurrentTime = 0;
		bIsPlayDamageAnim = true;

		EnemyState = EEnemyState::Attack;
	}
	else
	{
		MyOwner->SetBodyMaterial(DamageMaterial);
	}
}

/// <summary>
/// Die ������ �� ȣ��Ǵ� �Լ�
/// - ���� �ð����� �������ٰ� �������.
/// </summary>
void UJHEnemyFSM::DieState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if(CurrentTime > DieTime)
	{
		CurrentTime = 0;
		MyOwner->Destroy();
	}

	FVector Dir = FVector::DownVector * DieMoveSpeed;
	MyOwner->SetActorLocation(MyOwner->GetActorLocation() + Dir * GetWorld()->DeltaTimeSeconds );
}

/// <summary>
/// ������ ������ �� ȣ��Ǵ� �Լ�
/// - HP ���ҽ�Ű��, HP�� ���� ���� Damage, Die ���·� ��ȯ
/// </summary>
void UJHEnemyFSM::OnDamageProcess()
{

	if(Hp <=0) return;
	
	Hp--;

	float HpRate = Hp / MaxHp;

	// ü���� ������ 
	if(Hp <= 0)
	{
		// Die ���·� ��ȯ
		EnemyState = EEnemyState::Die;
	}
	// ���� �������� ����, ������ �ִϸ��̼��� ������ ���� ���ٸ�
	else if(HpRate <= DamageRate && !bIsPlayDamageAnim)
	{
        // Damage ���·� ��ȯ
        EnemyState = EEnemyState::Damage;
		
	}

	//UE_LOG(LogTemp, Warning, TEXT("Enemy Damage : %d , %f, %f"), Hp, MaxHp, HpRate);

	OnChangedHp.Broadcast(Hp);
}

