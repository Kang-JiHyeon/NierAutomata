// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemyFSM.h"
#include "JHEnemy.h"
#include "JHBombSkill.h"
#include "JHMissileSkill.h"
#include "JHBossSkillManager.h"
#include "JHBossAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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

	// 보스의 HP 초기화
	Hp = MaxHp;

	MyOwner = Cast<AJHEnemy>(GetOwner());
	if (MyOwner != nullptr)
	{
		UAnimInstance* AnimInstance = MyOwner->SkeletalMeshComp->GetAnimInstance();
		BossAnim = Cast<UJHBossAnimInstance>(AnimInstance);
		
		// todo : 불타는 파티클로 바꿔야 함
		//DefaultMaterial = MyOwner->GetBodyMaterial();
		
		PsFireComp = MyOwner->PsFireComp;
		//PsFireComp->AttachToComponent(MyOwner->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		PsFireComp->SetActive(false);
	}

}


// Called every frame
void UJHEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString LogMsg = UEnum::GetValueAsString(EnemyState);
	//GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, LogMsg);

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
/// Idle 상태일 때 호출되는 함수
/// - 일정 시간동안 대기하다가 Move 상태로 전환
/// </summary>
void UJHEnemyFSM::IdleState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if (CurrentTime > IdleDelayTime)
	{
		CurrentTime = 0;
		EnemyState = EEnemyState::Move;
		OnChangeAnimState();
	}

}

/// <summary>
/// Move 상태일 때 호출되는 함수
/// - 타켓 위치로 이동하고 Attack 상태로 전환
/// - todo : 타겟 위치 지정 필요!!
/// </summary>
void UJHEnemyFSM::MoveState()
{
	// todo : 이동 위치 재지정 필요
	
	// 중앙으로 이동
	FVector CurrPos = MyOwner->GetActorLocation();
	FVector DestPos = FVector::Zero() + FVector::UpVector * CurrPos.Z;
	FVector Dir = DestPos - CurrPos;

	if (Dir.Size() <= AttackRange) {
		MyOwner->SetActorLocation(DestPos);
		bIsMove = true;

		EnemyState = EEnemyState::Attack;

		OnChangeAnimState();
		OnChangeAttackPlay(true);
	}

	Dir.Normalize();

	// 이동
	MyOwner->SetActorLocation(CurrPos + Dir * MoveSpeed * GetWorld()->DeltaTimeSeconds);
	// 회전
	UKismetMathLibrary::FindLookAtRotation(CurrPos, DestPos);
}

/// <summary>
/// Attack 상태일 때 호출되는 함수
/// - 공격 시간동안 공격하다가 대기 상태로 전환
/// </summary>
void UJHEnemyFSM::AttackState()
{
	SkillManager->OnAttack();
}

/// <summary>
/// Damage 상태일 때 호출되는 함수
/// - 일정 시간동안 Damage 상태를 지속한다.
/// </summary>
void UJHEnemyFSM::DamageState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if (CurrentTime > DamageTime)
	{
		CurrentTime = 0;
		EnemyState = EEnemyState::Attack;
		SkillManager->SetAttackPlay(true);
		OnChangeAnimState();
	}
}

/// <summary>
/// Die 상태일 때 호출되는 함수
/// - 일정 시간동안 내려갔다가 사라진다.
/// </summary>
void UJHEnemyFSM::DieState()
{
	FVector Dir = FVector::DownVector * DieMoveSpeed;
	MyOwner->SetActorLocation(MyOwner->GetActorLocation() + Dir * GetWorld()->DeltaTimeSeconds );
	
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if(CurrentTime > DieTime)
	{
		MyOwner->Destroy();
	}
}



void UJHEnemyFSM::OnChangeAnimState()
{
	if (BossAnim != nullptr)
	{
		BossAnim->AnimState = EnemyState;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BossAnimInstance가 null입니다."));
	}
}

void UJHEnemyFSM::OnChangeAttackPlay(bool bPlay)
{
	if (BossAnim != nullptr)
	{
		// Attack 상태일 때 공격 재생 중이지 않으면 
		if (EnemyState == EEnemyState::Attack)
		{
			BossAnim->bAttackPlay = bPlay;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BossAnimInstance가 null입니다."));
	}
}

/// <summary>
/// 공격을 당했을 때 호출되는 함수
/// - HP 감소시키고, HP의 값에 따라 Damage, Die 상태로 전환
/// </summary>
void UJHEnemyFSM::OnDamageProcess(int32 Damage)
{
	if(Hp <=0) return;
	
	Hp -= Damage;

	float HpRate = Hp / MaxHp;

	// 체력이 없으면 
	if(Hp <= 0)
	{
		// Skill 초기화
		SkillManager->OnInitialize();

		// Die 상태로 전환
		EnemyState = EEnemyState::Die;
		CurrentTime = 0;
		OnChangeAnimState();
		
		// 폭발 NS 활성화
		MyOwner->NsExplosionComp->Activate(true);
		PsFireComp->Activate(true);

		// 폭발 사운드 재생
		UGameplayStatics::PlaySound2D(GetWorld(), MyOwner->ExplosionSound);
		UGameplayStatics::PlaySound2D(GetWorld(), MyOwner->ScreamSound);

	}
	// 일정 비율보다 낮고, 데미지 애니메이션을 시작한 적이 없다면
	else if(HpRate < DamageRate && !bIsPlayDamageAnim)
	{
		// Skill 초기화
		SkillManager->OnInitialize();
		// Skill 의 난이도를 Hard 로 변경
		SkillManager->OnToggleSkillLevel();


		bIsPlayDamageAnim = true;

		// Damage 파티클 재생
		PsFireComp->SetActive(true);
		UE_LOG(LogTemp, Warning, TEXT("Fire Particle Start!"));

		// 1초 뒤에 Damage 파티클 비활성화
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (PsFireComp)
			{
				PsFireComp->SetActive(false);
				UE_LOG(LogTemp, Warning, TEXT("Fire Particle End!"));

			}
		}, 1.0f, false);

        // Damage 상태로 전환
        EnemyState = EEnemyState::Damage;
		OnChangeAnimState();

		// Sound 재생
		UGameplayStatics::PlaySound2D(GetWorld(), MyOwner->ExplosionSound);
		UGameplayStatics::PlaySound2D(GetWorld(), MyOwner->ScreamSound);



	}

	//UE_LOG(LogTemp, Warning, TEXT("Enemy Damage : %d , %f, %f"), Hp, MaxHp, HpRate);

	OnChangedHp.Broadcast(Hp);
}