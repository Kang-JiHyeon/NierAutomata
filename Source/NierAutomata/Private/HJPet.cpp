// Fill out your copyright notice in the Description page of Project Settings.

#include "HJPet.h"
#include "Components/BoxComponent.h"


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

}

// Called when the game starts or when spawned
void AHJPet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJPet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

