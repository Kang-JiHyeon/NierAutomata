// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemy.h"
#include "JHEnemyFSM.h"
#include "JHBombSkill.h"
#include "JHBomb.h"
#include "JHMissileSkill.h"
#include "JHMissile.h"
#include "JHLaserBeamSkill.h"
#include "JHLaserBeam.h"
#include "JHSpiralMoveSkill.h"
#include "JHBossSkillManager.h"
#include "JHBossAnimInstance.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
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


	// todo : 캡슐 충돌체 설정
	//BodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	//BodyMeshComp->SetupAttachment(RootComponent);
	//BodyMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	//// Top - Capsule
	//ConstructorHelpers::FObjectFinder<UStaticMesh> TopMeshFinder(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	//if (TopMeshFinder.Succeeded())
	//	BodyMeshComp->SetStaticMesh(TopMeshFinder.Object);
	//BodyMeshComp->SetRelativeLocation(FVector(0, 0, -50));

	// BossSkillManager
	BossSkillManager = CreateDefaultSubobject<UJHBossSkillManager>(TEXT("BossSkillManager"));

	// FSM
	Fsm = CreateDefaultSubobject<UJHEnemyFSM>(TEXT("EnemyFSM"));
	Fsm->SkillManager = BossSkillManager;

	// Bottom
	BottomMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom Mesh"));
	BottomMeshComp->SetupAttachment(RootComponent);
	BottomMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	// Top : SkeletalMeshComp
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(RootComponent);
	SkeletalMeshComp->SetRelativeScale3D(FVector(0.3f));
	SkeletalMeshComp->SetRelativeLocation(FVector(0, 0, -50));
	SkeletalMeshComp->SetRelativeRotation(FRotator(0, -90, 0));
	
	// SkeletalMesh Collision
	SkeletalMeshComp->SetGenerateOverlapEvents(true);
	SkeletalMeshComp->SetCollisionProfileName(TEXT("Enemy"));
	//SkeletalMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AJHEnemy::OnDamageProcess);

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

	// Bottom - Cylinder
	ConstructorHelpers::FObjectFinder<UStaticMesh> BottomMeshFinder(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	if (BottomMeshFinder.Succeeded())
		BottomMeshComp->SetStaticMesh(BottomMeshFinder.Object);
	BottomMeshComp->SetRelativeLocation(FVector(0, 0, -50));

	// Bomb
	BombSkill = CreateDefaultSubobject<UJHBombSkill>(TEXT("Bomb Skill"));
	BombSkill->SetupAttachment(BottomMeshComp);
	
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
	LaserBeamSkill->SetupAttachment(BottomMeshComp);

	ConstructorHelpers::FClassFinder<AJHLaserBeam> LaserBeamFinder(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Kang/BP_JHLaserBeam.BP_JHLaserBeam_C'"));
	if (LaserBeamFinder.Succeeded())
	{
		LaserBeamSkill->SkillFactory = LaserBeamFinder.Class;
	}

	// SpiralMoveSkill
	SpiralMoveSkill = CreateDefaultSubobject<UJHSpiralMoveSkill>(TEXT("SpiralMove Skill"));
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

//void AJHEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//	Super::NotifyActorBeginOverlap(OtherActor);
//
//}


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
	FRotator Rot = BottomMeshComp->GetRelativeRotation() + FRotator(0, RotSpeed * GetWorld()->DeltaTimeSeconds, 0);

	BottomMeshComp->SetRelativeRotation(Rot);

	//UE_LOG(LogTemp, Warning, TEXT("RotateSpinBottom!"));
}

void AJHEnemy::SetBodyMaterial(UMaterialInterface* NewMaterial)
{
	if(SkeletalMeshComp == nullptr || SkeletalMeshComp->GetMaterial(0) == NewMaterial) return;

	SkeletalMeshComp->SetMaterial(0, NewMaterial);
}

UMaterialInterface* AJHEnemy::GetBodyMaterial()
{
	if(SkeletalMeshComp == nullptr)
		return nullptr;
	
	return SkeletalMeshComp->GetMaterial(0);
}

void AJHEnemy::OnDamageProcess(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어의 무기라면 제거하지 않음

	// TODO : Weapon 중복 Overlap 해결
	if (OtherActor->Tags.Contains(TEXT("PlayerWeapon")))
	{
		Fsm->OnDamageProcess(1);
		UE_LOG(LogTemp, Warning, TEXT("PlayerWeapon Overlap!"));
	}
	else if (OtherActor->Tags.Contains(TEXT("PetBullet")))
	{
		Fsm->OnDamageProcess(1);
		// 공격 제거
		OtherActor->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("PetBullet Overlap!"));
	}
	else if (OtherActor->Tags.Contains(TEXT("PetLaser")))
	{
		Fsm->OnDamageProcess(10);
		OtherActor->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("PetLaser Overlap!"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Overlap : %s"), *OtherActor->GetName());
}