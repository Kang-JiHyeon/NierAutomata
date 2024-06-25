// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemy.h"
#include "JHEnemyFSM.h"
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

	//ConstructorHelpers::FObjectFinder<UMaterial> TempMat(TEXT("/Script/Engine.Material'/Engine/MapTemplates/Materials/BasicAsset02.BasicAsset02'"));
	//if (TempMat.Succeeded())
	//	MeshComp->SetMaterial(0, TempMat.Object);

	// FSM
	Fsm = CreateDefaultSubobject<UJHEnemyFSM>(TEXT("EnemyFSM"));
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

