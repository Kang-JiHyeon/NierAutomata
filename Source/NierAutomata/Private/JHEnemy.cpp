// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemy.h"
#include "JHEnemyFSM.h"
#include "JHBombSkill.h"
#include "JHBomb.h"
#include "JHMissile.h"
#include "JHLaserBeam.h"
#include "JHMissileSkill.h"
#include "JHLaserBeamSkill.h"
#include "JHSpiralMoveSkill.h"
#include "JHBossSkillManager.h"
#include "JHBossAnimInstance.h"
#include "JHEnemyDamageUI.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AJHEnemy::AJHEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(RootCapsuleComp);
	// 크기
	RootCapsuleComp->SetCapsuleHalfHeight(50);
	RootCapsuleComp->SetCapsuleRadius(25);

	// Collision
	SphereTopComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp_Top"));
	SphereTopComp->SetupAttachment(RootComponent);
	SphereTopComp->SetRelativeLocation(FVector(0, 0, 40.0));
	SphereTopComp->SetSphereRadius(25);
	SphereTopComp->SetCollisionProfileName(TEXT("Enemy"));
	SphereTopComp->OnComponentBeginOverlap.AddDynamic(this, &AJHEnemy::OnDamageProcess);

	SphereBottomComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp_Bottom"));
	SphereBottomComp->SetupAttachment(RootComponent);
	SphereBottomComp->SetRelativeLocation(FVector(0, 0, -25));
	SphereBottomComp->SetSphereRadius(50);
	SphereBottomComp->SetCollisionProfileName(TEXT("Enemy"));
	SphereBottomComp->OnComponentBeginOverlap.AddDynamic(this, &AJHEnemy::OnDamageProcess);

	// BossSkillManager
	BossSkillManager = CreateDefaultSubobject<UJHBossSkillManager>(TEXT("BossSkillManager"));

	// FSM
	Fsm = CreateDefaultSubobject<UJHEnemyFSM>(TEXT("EnemyFSM"));
	Fsm->SkillManager = BossSkillManager;

	// Bottom
	BottomSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("BottomSceneComp"));
	BottomSceneComp->SetupAttachment(RootComponent);
	BottomSceneComp->SetRelativeLocation(FVector(0, 0, -50));

	// Top : SkeletalMeshComp
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(RootComponent);
	SkeletalMeshComp->SetRelativeScale3D(FVector(0.3f));
	SkeletalMeshComp->SetRelativeLocation(FVector(0, 0, -50));
	SkeletalMeshComp->SetRelativeRotation(FRotator(0, -90, 0));
	
	// SkeletalMesh Collision
	SkeletalMeshComp->SetGenerateOverlapEvents(true);
	SkeletalMeshComp->SetCollisionProfileName(TEXT("Enemy"));

	// SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/Beauvoir/NierAutomata_Beauvoir.NierAutomata_Beauvoir'"));
	if (SkeletalMeshFinder.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(SkeletalMeshFinder.Object);
	}
	// Anim Class
	ConstructorHelpers::FClassFinder<UJHBossAnimInstance> AnimFinder(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/Beauvoir/ABP_JHBeauvoir.ABP_JHBeauvoir_C'"));
	if (AnimFinder.Succeeded())
	{
		SkeletalMeshComp->SetAnimClass(AnimFinder.Class);
	}

	// Bomb
	BombSkill = CreateDefaultSubobject<UJHBombSkill>(TEXT("Bomb Skill"));
	BombSkill->SetupAttachment(BottomSceneComp);
	
	for (int32 i = 0; i < BombSkill->BombCount; i++) {
		FString FirePosName = FString::Printf(TEXT("FirePos_%d"), i);
		USceneComponent* TempFirePos = CreateDefaultSubobject<USceneComponent>(*FirePosName);

		TempFirePos->SetupAttachment(BombSkill);
		TempFirePos->SetRelativeRotation(FRotator(50, i * (360 / BombSkill->BombCount), 0));

		BombSkill->FirePositions.Add(TempFirePos);
	}

	ConstructorHelpers::FClassFinder<AJHBomb> BombFinder(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Kang/BP_JHBomb.BP_JHBomb_C'"));
	if (BombFinder.Succeeded())
	{
		BombSkill->SkillFactory = BombFinder.Class;
	}

	// Missile
	MissileSkill = CreateDefaultSubobject<UJHMissileSkill>(TEXT("Missile Skill"));
	MissileSkill->SetupAttachment(RootComponent);

	SequentialMissileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Missile Arrow (Sequential)"));
	SequentialMissileArrow->SetupAttachment(MissileSkill);
	SequentialMissileArrow->SetRelativeLocation(FVector(0, 0, 50));
	SequentialMissileArrow->SetRelativeRotation(FRotator(90, 0, 0));

	OnceMissileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Missile Arrow (Once)"));
	OnceMissileArrow->SetupAttachment(MissileSkill);
	OnceMissileArrow->SetRelativeLocation(FVector(0, 0, -35));

	MissileSkill->SequentilSkillArrow = SequentialMissileArrow;
	MissileSkill->OnceSkillArrow = OnceMissileArrow;
	
	ConstructorHelpers::FClassFinder<AJHMissile> SequentialMissileFinder(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Kang/BP_JHMissile.BP_JHMissile_C'"));
	if (SequentialMissileFinder.Succeeded())
	{
		MissileSkill->SequentialMissileFactory = SequentialMissileFinder.Class;
	}

	ConstructorHelpers::FClassFinder<AJHMissile> AtOnceMissileFinder(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Kang/BP_JHMissile_AtOnce.BP_JHMissile_AtOnce_C'"));
	if (AtOnceMissileFinder.Succeeded())
	{
		MissileSkill->OnceMissileFactory = AtOnceMissileFinder.Class;
	}

	// LaserBeam
	LaserBeamSkill = CreateDefaultSubobject<UJHLaserBeamSkill>(TEXT("LaserBeam Skill"));
	LaserBeamSkill->SetupAttachment(BottomSceneComp);

	ConstructorHelpers::FClassFinder<AJHLaserBeam> LaserBeamFinder(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Kang/BP_JHLaserBeam.BP_JHLaserBeam_C'"));
	if (LaserBeamFinder.Succeeded())
	{
		LaserBeamSkill->SkillFactory = LaserBeamFinder.Class;
	}

	// SpiralMoveSkill
	SpiralMoveSkill = CreateDefaultSubobject<UJHSpiralMoveSkill>(TEXT("SpiralMove Skill"));

	// Particle System Component
	PsFireComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PSDamageComp"));
	PsFireComp->SetupAttachment(RootComponent);

	// Niagara System
	NsExplosionComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NSDieComp"));
	NsExplosionComp->SetupAttachment(RootComponent);

	// Damage UI
	ConstructorHelpers::FClassFinder<UJHEnemyDamageUI> DamageUIFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Kang/BP_JHEnemyDamageUI.BP_JHEnemyDamageUI_C'"));
	if (DamageUIFinder.Succeeded())
	{
		DamageUIFactory = DamageUIFinder.Class;
	}

	// Audio
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
	AudioComp->Activate(false);
}

// Called when the game starts or when spawned
void AJHEnemy::BeginPlay()
{
	Super::BeginPlay();

	MoveTargetPos += FVector(0, 0, GetActorLocation().Z);
	RotateTarget = GetWorld()->GetFirstPlayerController()->GetPawn();

	FVector CurrPos = this->GetActorLocation();
	RotateTargetPos = RotateTarget->GetActorLocation();
	
	RotateTargetPos.Z = 0;
	CurrPos.Z = 0;

	RotateTargetDirection = RotateTargetPos - CurrPos;
	RotateTargetDirection.Normalize();
}

// Called every frame
void AJHEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJHEnemy::SetMovement(bool bValue)
{
	bIsMove = bValue;

}
void AJHEnemy::SetLockAt(bool bValue)
{
	bIsLookAt = bValue;
}

void AJHEnemy::SetRotSpeed(float Value)
{
	RotSpeed = Value;
}

/// <summary>
/// 이동 함수
/// </summary>
void AJHEnemy::Movement()
{
	// 이동 중이 아니라면 종료
	if (!bIsMove) return;

	// 타겟의 위치로 이동
	FVector CurrPos = GetActorLocation();
	FVector Dir = MoveTargetPos  - CurrPos;

	// 타겟 위치에 가까이 왔다면 타겟으로 위치 지정
	if (Dir.Length() < 10)
	{
		SetActorLocation(MoveTargetPos);
		bIsMove = false;
	}
	else
	{
		Dir.Normalize();
		SetActorLocation(CurrPos + Dir * MoveSpeed * GetWorld()->DeltaTimeSeconds);
	}
}

void AJHEnemy::Movement(FVector TargetPosition)
{
	// 이동 중이 아니라면 종료
	if (!bIsMove) return;

	// 타겟의 위치로 이동
	FVector CurrPos = GetActorLocation();
	FVector Dir = TargetPosition - CurrPos;

	// 타겟 위치에 가까이 왔다면 타겟으로 위치 지정
	if (Dir.Length() < 10)
	{
		SetActorLocation(TargetPosition);
		bIsMove = false;
	}
	else
	{
		Dir.Normalize();
		SetActorLocation(CurrPos + Dir * MoveSpeed * GetWorld()->DeltaTimeSeconds);
	}
}

/// <summary>
/// 회전 함수
/// </summary>
void AJHEnemy::RotateLookAt()
{
    FVector CurrPos = this->GetActorLocation();
    FVector TargetPos = RotateTarget->GetActorLocation();

    CurrPos.Z = 0;
    TargetPos.Z = 0;

    FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(CurrPos, TargetPos);
    FRotator InterpRot = UKismetMathLibrary::RInterpTo(this->GetActorRotation(), TargetRot, GetWorld()->DeltaTimeSeconds, InterpSpeed);

    SetActorRotation(InterpRot);
}

///// <summary>
///// 특정 좌표를 바라보는 방향으로 회전
///// </summary>
///// <param name="TargetPosition"></param>
//void AJHEnemy::RotateTarget(FVector TargetPosition)
//{
//	if (!bIsRotatePosition) return;
//
//	FVector CurrPos = this->GetActorLocation();
//	CurrPos.Z = 0;
//	TargetPosition.Z = 0;
//
//	UKismetMathLibrary::FindLookAtRotation(CurrPos, TargetPosition);
//}

/// <summary>
/// 몸 전체 회전
/// </summary>
void AJHEnemy::RotateSpinBody()
{
	FRotator Rot = this->GetActorRotation() + FRotator(0, RotSpeed * GetWorld()->DeltaTimeSeconds, 0);

	this->SetActorRotation(Rot);

	//UE_LOG(LogTemp, Warning, TEXT("RotateSpinBody!"));
}

/// <summary>
/// 몸 하단 회전
/// </summary>
void AJHEnemy::RotateSpinBottom()
{
	FRotator Rot = BottomSceneComp->GetRelativeRotation() + FRotator(0, RotSpeed * GetWorld()->DeltaTimeSeconds, 0);

	BottomSceneComp->SetRelativeRotation(Rot);
}

void AJHEnemy::OnDamageProcess(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// HP 
	int32 Damage = 0;
	// TODO : Weapon 중복 Overlap 해결
	// 플레이어의 무기라면 제거하지 않음
	if (OtherActor->Tags.Contains(TEXT("PlayerWeapon")))
	{
		Damage = UKismetMathLibrary::RandomIntegerInRange(5, 10);;
		Fsm->OnDamageProcess(Damage);
		UE_LOG(LogTemp, Warning, TEXT("PlayerWeapon Overlap!"));
	}
	else if (OtherActor->Tags.Contains(TEXT("PetBullet")))
	{
		Damage = UKismetMathLibrary::RandomIntegerInRange(1, 3);
		Fsm->OnDamageProcess(Damage);
		/*OtherActor->Destroy();*/
		UE_LOG(LogTemp, Warning, TEXT("PetBullet Overlap!"));
	}
	else if (OtherActor->Tags.Contains(TEXT("PetLaser")))
	{
		Damage = UKismetMathLibrary::RandomIntegerInRange(20, 40);
		Fsm->OnDamageProcess(Damage);
		OtherActor->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("PetLaser Overlap!"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Overlap : %s"), *OtherActor->GetName());

	if (Damage <= 0) return;

	// Damage UI 생성
	if (DamageUIFactory != nullptr)
	{
		// 위젯 생성
		auto* DamageUI = Cast<UJHEnemyDamageUI>(CreateWidget(GetWorld(), DamageUIFactory));

		if (DamageUI != nullptr)
		{
			DamageUI->SetTracePosision(OtherComp->GetComponentLocation());
			DamageUI->SetTextDamage(Damage);
			DamageUI->AddToViewport(1);
		}
	}
}

ESkillType AJHEnemy::GetSkillType()
{

	// 보스가 Attack 중이라면
	// 현재 스킬 타입을 반환한다.
	// Attack 중이 아니라면
	// None을 반환한다.

	if (Fsm->EnemyState == EEnemyState::Attack)
	{
		return BossSkillManager->CurrSkillType;
	}
	
	return ESkillType::None;
}

void AJHEnemy::SetSoundBase(USoundBase* SoundBase)
{
	AudioComp->Sound = SoundBase;
}

void AJHEnemy::SetActiveSound(bool bPlay)
{
	if(bPlay)
		AudioComp->Play();
	else
		AudioComp->Stop();
}

void AJHEnemy::OnEndAttackSkill()
{
	BossSkillManager->UpdatePattern();
}