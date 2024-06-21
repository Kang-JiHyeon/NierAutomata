// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBombSkill.h"
#include "JHBomb.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AJHBombSkill::AJHBombSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(capsuleComp);
	// 크기
	capsuleComp->SetCapsuleHalfHeight(50);
	capsuleComp->SetCapsuleRadius(50);
	
	// 충돌체
	//capsuleComp->SetCollisionProfileName(TEXT("NoCollision"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(capsuleComp);
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));


	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (tempMesh.Succeeded())
		meshComp->SetStaticMesh(tempMesh.Object);
	//meshComp->SetRelativeLocation(FVector(0, 0, -50));

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("/Script/Engine.Material'/Engine/MapTemplates/Materials/BasicAsset02.BasicAsset02'"));
	if (tempMat.Succeeded())
		meshComp->SetMaterial(0, tempMat.Object);

	ConstructorHelpers::FClassFinder<AJHBomb> tempBomb(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_JHBomb.BP_JHBomb_C'"));
	if (tempBomb.Succeeded())
		bombFactory = tempBomb.Class;

	// 발사 위치
	for (int i = 0; i < bombCount; i++) {
		//firePositions.Add(CreateDefaultSubobject<USceneComponent>(TEXT("FirePos")));
		UE_LOG(LogTemp, Warning, TEXT("sdfsdf"));
		//firePositions.Emplace(CreateDefaultSubobject<USceneComponent>(TEXT("FirePos %d"), i));
	}

	for (int i = 0; i < firePositions.Num(); i++) {

		firePositions[i]->SetupAttachment(capsuleComp);
		firePositions[i]->SetRelativeRotation(FRotator(45, 0, 360 / i));
	}


}

// Called when the game starts or when spawned
void AJHBombSkill::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJHBombSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

