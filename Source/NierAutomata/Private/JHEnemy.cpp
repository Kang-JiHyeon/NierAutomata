// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemy.h"
#include "JHEnemyFSM.h"
#include "JHBombSkill.h"
#include "JHBomb.h"
#include "JHMissileSkill.h"
#include "JHMissile.h"
#include "JHBossSkillManager.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AJHEnemy::AJHEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComp);
	// 크기
	CapsuleComp->SetCapsuleHalfHeight(50);
	CapsuleComp->SetCapsuleRadius(25);
	CapsuleComp->SetWorldScale3D(FVector(5, 5, 5));

	// todo : 캡슐 충돌체 설정
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(CapsuleComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	if (TempMesh.Succeeded())
		MeshComp->SetStaticMesh(TempMesh.Object);
	MeshComp->SetRelativeLocation(FVector(0, 0, -50));

	// BossSkillManager
	BossSkillManager = CreateDefaultSubobject<UJHBossSkillManager>(TEXT("BossSkillManager"));

	// FSM
	Fsm = CreateDefaultSubobject<UJHEnemyFSM>(TEXT("EnemyFSM"));
	Fsm->SkillManager = BossSkillManager;

	 //todo : 부모 설정?

	// Bomb
	UJHBombSkill* BombSkill = CreateDefaultSubobject<UJHBombSkill>(TEXT("BombSkill"));
	BombSkill->SetupAttachment(CapsuleComp);

	
	for (int32 i = 0; i < BombSkill->BombCount; i++) {
		FString FirePosName = FString::Printf(TEXT("FirePos_%d"), i);
		USceneComponent* TempFirePos = CreateDefaultSubobject<USceneComponent>(*FirePosName);

		TempFirePos->SetRelativeRotation(FRotator(50, i * (360 / BombSkill->BombCount), 0));
		TempFirePos->SetupAttachment(BombSkill);

		BombSkill->FirePositions.Add(TempFirePos);
	}

	
	ConstructorHelpers::FClassFinder<AJHBomb> TempBomb(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Kang/BP_JHBomb.BP_JHBomb_C'"));
	if (TempBomb.Succeeded())
	{
		BombSkill->BombFactory = TempBomb.Class;
	}

	// Missile
	UJHMissileSkill* MissileSkill = CreateDefaultSubobject<UJHMissileSkill>(TEXT("MissileSkill"));
	MissileSkill->SetupAttachment(CapsuleComp);

	UArrowComponent* MissileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Missile Arrow"));
	MissileArrow->SetupAttachment(MissileSkill);
	MissileArrow->SetRelativeLocation(FVector(0, 0, 50));
	MissileArrow->SetRelativeRotation(FRotator(90, 0, 0));

	MissileSkill->SkillPosition = MissileArrow;

	ConstructorHelpers::FClassFinder<AJHMissile> TempMissile(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Kang/BP_JHMissile.BP_JHMissile_C'"));
	if (TempMissile.Succeeded())
	{
		MissileSkill->SkillFactory = TempMissile.Class;
	}


	// BossSkillManager
	BossSkillManager->BombSkill = BombSkill;
	BossSkillManager->MissileSkill = MissileSkill;

}

// Called when the game starts or when spawned
void AJHEnemy::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void AJHEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

