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

	// todo : 캡슐 충돌체 설정
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(CapsuleComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	if (TempMesh.Succeeded())
		MeshComp->SetStaticMesh(TempMesh.Object);
	MeshComp->SetRelativeLocation(FVector(0, 0, -50));

	// FSM
	Fsm = CreateDefaultSubobject<UJHEnemyFSM>(TEXT("EnemyFSM"));

	// BossSkillManager
	BossSkillManager = CreateDefaultSubobject<UJHBossSkillManager>(TEXT("BossSkillManager"));

	// todo : 부모 설정?

	// Bomb
	BombSkill = CreateDefaultSubobject<UJHBombSkill>(TEXT("BombSkill"));
	BombSkill->SetupAttachment(CapsuleComp);
	
	AttackSkills.Add(BombSkill);

	
	for (int32 i = 0; i < BombCount; i++) {
		FString FirePosName = FString::Printf(TEXT("FirePos_%d"), i);
		USceneComponent* TempFirePos = CreateDefaultSubobject<USceneComponent>(*FirePosName);

		TempFirePos->SetRelativeRotation(FRotator(50, i * (360 / BombCount), 0));
		TempFirePos->SetupAttachment(BombSkill);

		FirePositions.Add(TempFirePos);
	}

	ConstructorHelpers::FClassFinder<AJHBomb> TempBomb(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_JHBomb.BP_JHBomb_C'"));
	if (TempBomb.Succeeded())
	{
		BombFactory = TempBomb.Class;
	}

	// Missile
	MissileSkill = CreateDefaultSubobject<UJHMissileSkill>(TEXT("MessileSkill"));
	MissileSkill->SetupAttachment(CapsuleComp);

	MissileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Messile Arrow"));
	MissileArrow->SetupAttachment(MissileSkill);
	MissileArrow->SetRelativeLocation(FVector(0, 0, 50));
	MissileArrow->SetRelativeRotation(FRotator(90, 0, 0));

	ConstructorHelpers::FClassFinder<AJHMissile> TempMissile(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_JHMissile.BP_JHMissile_C'"));
	if (TempMissile.Succeeded())
	{
		MissileFactory = TempMissile.Class;
	}


	// AttackSkill
	AttackSkills.Add(BombSkill);
	AttackSkills.Add(MissileSkill);



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

