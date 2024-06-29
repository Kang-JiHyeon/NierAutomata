// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemy.h"
#include "JHEnemyFSM.h"
#include "JHBombSkill.h"
#include "JHBomb.h"
#include "JHMissileSkill.h"
#include "JHMissile.h"
#include "JHLaserBeamSkill.h"
#include "JHBossSkillManager.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"


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
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootCapsuleComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	if (MeshFinder.Succeeded())
		MeshComp->SetStaticMesh(MeshFinder.Object);
	MeshComp->SetRelativeLocation(FVector(0, 0, -50));

	// BossSkillManager
	BossSkillManager = CreateDefaultSubobject<UJHBossSkillManager>(TEXT("BossSkillManager"));

	// FSM
	Fsm = CreateDefaultSubobject<UJHEnemyFSM>(TEXT("EnemyFSM"));
	Fsm->SkillManager = BossSkillManager;

	 //todo : 부모 설정?

	// Bomb
	UJHBombSkill* BombSkill = CreateDefaultSubobject<UJHBombSkill>(TEXT("BombSkill"));
	BombSkill->SetupAttachment(RootCapsuleComp);

	
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
		BombSkill->BombFactory = BombFinder.Class;
	}

	// Missile
	UJHMissileSkill* MissileSkill = CreateDefaultSubobject<UJHMissileSkill>(TEXT("MissileSkill"));
	MissileSkill->SetupAttachment(RootCapsuleComp);

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
	UJHLaserBeamSkill* LaserBeamSkill = CreateDefaultSubobject<UJHLaserBeamSkill>(TEXT("LaserBeamSkill"));
	LaserBeamSkill->SetupAttachment(RootCapsuleComp);

	USplineComponent* SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(LaserBeamSkill);
	LaserBeamSkill->SplineComponent = SplineComponent;


	ConstructorHelpers::FObjectFinder<UStaticMesh> LineMeshFinder(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	if (LineMeshFinder.Succeeded())
	{
		LaserBeamSkill->LineMesh = LineMeshFinder.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterial> LineMaterialFinder(TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Water_Ocean.M_Water_Ocean'"));
	if (LineMaterialFinder.Succeeded())
	{
		LaserBeamSkill->LineMaterial = LineMaterialFinder.Object;
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
	
}

// Called every frame
void AJHEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

