// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemy.h"
#include "JHEnemyFSM.h"
#include "JHBombSkill.h"
#include "JHBomb.h"
#include "JHMissileSkill.h"
#include "JHMissile.h"
#include "JHLaserBeamSkill.h"
#include "JHLaserBeam.h"
#include "JHBossSkillManager.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"


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
	RootCapsuleComp->SetWorldScale3D(FVector(5, 5, 5));

	// todo : 캡슐 충돌체 설정
	BodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMeshComp->SetupAttachment(RootComponent);
	BodyMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	// Top - Capsule
	ConstructorHelpers::FObjectFinder<UStaticMesh> TopMeshFinder(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	if (TopMeshFinder.Succeeded())
		BodyMeshComp->SetStaticMesh(TopMeshFinder.Object);
	BodyMeshComp->SetRelativeLocation(FVector(0, 0, -50));

	// Bottom
	BottomMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom Mesh"));
	BottomMeshComp->SetupAttachment(RootComponent);
	BottomMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	// Bottom - Cylinder
	ConstructorHelpers::FObjectFinder<UStaticMesh> BottomMeshFinder(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	if (BottomMeshFinder.Succeeded())
		BottomMeshComp->SetStaticMesh(BottomMeshFinder.Object);
	BottomMeshComp->SetRelativeLocation(FVector(0, 0, -50));

	// BossSkillManager
	BossSkillManager = CreateDefaultSubobject<UJHBossSkillManager>(TEXT("BossSkillManager"));

	// FSM
	Fsm = CreateDefaultSubobject<UJHEnemyFSM>(TEXT("EnemyFSM"));
	Fsm->SkillManager = BossSkillManager;

	 //todo : 부모 설정?

	// Bomb
	BombSkill = CreateDefaultSubobject<UJHBombSkill>(TEXT("Bomb Skill"));
	BombSkill->SetupAttachment(BottomMeshComp);

	
	for (int32 i = 0; i < BombSkill->BombCount; i++) {
		FString FirePosName = FString::Printf(TEXT("FirePos_%d"), i);
		USceneComponent* TempFirePos = CreateDefaultSubobject<USceneComponent>(*FirePosName);

		TempFirePos->SetRelativeRotation(FRotator(50, i * (360 / BombSkill->BombCount), 0));
		TempFirePos->SetupAttachment(BombSkill);

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

	UArrowComponent* MissileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Missile Arrow"));
	MissileArrow->SetupAttachment(MissileSkill);
	MissileArrow->SetRelativeLocation(FVector(0, 0, 50));
	MissileArrow->SetRelativeRotation(FRotator(90, 0, 0));

	MissileSkill->SkillPosition = MissileArrow;
	
	ConstructorHelpers::FClassFinder<AJHMissile> MissileFinder(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Kang/BP_JHMissile.BP_JHMissile_C'"));
	if (MissileFinder.Succeeded())
	{
		MissileSkill->SkillFactory = MissileFinder.Class;
	}

	// LaserBeam
	LaserBeamSkill = CreateDefaultSubobject<UJHLaserBeamSkill>(TEXT("LaserBeam Skill"));
	LaserBeamSkill->SetupAttachment(BottomMeshComp);

	ConstructorHelpers::FClassFinder<AJHLaserBeam> LaserBeamFinder(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Kang/BP_JHLaserBeam.BP_JHLaserBeam_C'"));
	if (LaserBeamFinder.Succeeded())
	{
		LaserBeamSkill->SkillFactory = LaserBeamFinder.Class;
	}

	// BossSkillManager
	BossSkillManager->BombSkill = BombSkill;
	BossSkillManager->MissileSkill = MissileSkill;
	BossSkillManager->LaserBeamSkill = LaserBeamSkill;
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

	UE_LOG(LogTemp, Warning, TEXT("RotateSpinBody!"));
}

/// <summary>
/// 몸 하단 회전
/// </summary>
void AJHEnemy::RotateSpinBottom()
{
	FRotator Rot = BottomMeshComp->GetRelativeRotation() + FRotator(0, RotSpeed * GetWorld()->DeltaTimeSeconds, 0);

	BottomMeshComp->SetRelativeRotation(Rot);

	UE_LOG(LogTemp, Warning, TEXT("RotateSpinBottom!"));
}