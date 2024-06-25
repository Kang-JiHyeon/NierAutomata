// Fill out your copyright notice in the Description page of Project Settings.

#include "HJPet.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HJMainCharacter.h"
#include "HJPetBullet.h"


// Sets default values
AHJPet::AHJPet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 생성 
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	// 루트 컴포넌트 지정 및 박스 크기 조정 
	SetRootComponent(compBox);
	compMesh->SetupAttachment(compBox);
	compBox->SetBoxExtent(FVector(30, 30, 30));

	// 메쉬 및 메테리얼 오브젝트 지정 
	ConstructorHelpers::FObjectFinder<UStaticMesh>
		tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial>
		tempMat(TEXT("/Script/Engine.Material'/Engine/MapTemplates/Materials/BasicAsset01.BasicAsset01'"));
	if (tempMat.Succeeded())
	{
		compMesh->SetMaterial(0, tempMat.Object);
	}

	// Bullet 엑터가 부착할 컴포넌트 생성 
	firePos = CreateDefaultSubobject<USceneComponent>(TEXT("FIRE"));
	firePos->SetupAttachment(compBox);
	firePos->SetRelativeLocation(FVector(150, 0, 0));

	// Bullet 공장의 실체 
	ConstructorHelpers::FClassFinder<AHJPetBullet>
		tempFire(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_HJPetBullet.BP_HJPetBullet_C'"));
	if (tempFire.Succeeded())
	{
		bulletFactory = tempFire.Class;
	}
}

// Called when the game starts or when spawned
void AHJPet::BeginPlay()
{
	Super::BeginPlay();
	// 타겟 메인캐릭터 찾기 
	target = UGameplayStatics::GetActorOfClass(GetWorld(), AHJMainCharacter::StaticClass());

}

// Called every frame
void AHJPet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 타겟을따라가야함
	FVector dir = target->GetActorLocation() + FVector(-200, 0, 300) - GetActorLocation();
	// 만약에 거리가 멀경우
	if (dir.Length() > 20)
	{
		dir.Normalize();
		FVector p0 = GetActorLocation();
		FVector vt = dir * speed * DeltaTime;
		SetActorLocation(p0 + vt);
	}
	// *중요한 문제* 거리가 충족되었을 때, 계속해서 충돌하는 오류 발생 // if 문으로 거리 설정 필요

	// 1초마다 Bullet이 자동으로 생성되도록 설정 
	currTime += DeltaTime;
	if (currTime > createTime)
	{
		AHJPetBullet* bullet = GetWorld()->SpawnActor<AHJPetBullet>(bulletFactory);
		bullet->SetActorLocation(firePos->GetComponentLocation());
		currTime = 0;
	}
}

