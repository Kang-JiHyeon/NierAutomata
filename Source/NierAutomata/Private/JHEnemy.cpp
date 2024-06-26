// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemy.h"
#include "JHEnemyFSM.h"
#include "JHBombSkill.h"
#include "JHBomb.h"
#include "JHMissileSkill.h"
#include "JHMissile.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AJHEnemy::AJHEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComp);
	// Å©±â
	CapsuleComp->SetCapsuleHalfHeight(50);
	CapsuleComp->SetCapsuleRadius(25);

	// todo : Ä¸½¶ Ãæµ¹Ã¼ ¼³Á¤
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(CapsuleComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	if (TempMesh.Succeeded())
		MeshComp->SetStaticMesh(TempMesh.Object);
	MeshComp->SetRelativeLocation(FVector(0, 0, -50));

	// FSM
	Fsm = CreateDefaultSubobject<UJHEnemyFSM>(TEXT("EnemyFSM"));

	// Bomb
	BombSkill = CreateDefaultSubobject<UJHBombSkill>(TEXT("BombSkill"));
	BombSkill->SetupAttachment(CapsuleComp);

	
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
	MessileSkill = CreateDefaultSubobject<UJHMissileSkill>(TEXT("MessileSkill"));
	MessileSkill->SetupAttachment(CapsuleComp);

	MessilePosition = CreateDefaultSubobject<USceneComponent>(TEXT("Messile Position"));
	MessilePosition->SetRelativeRotation(FRotator(90, 0, 0));
	MessilePosition->SetupAttachment(MessileSkill);

	ConstructorHelpers::FClassFinder<AJHMissile> TempMissile(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_JHMissile.BP_JHMissile_C'"));
	if (TempMissile.Succeeded())
	{
		MessileFactory = TempMissile.Class;
	}
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

