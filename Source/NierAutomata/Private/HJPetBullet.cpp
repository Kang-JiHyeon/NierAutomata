// Fill out your copyright notice in the Description page of Project Settings.


#include "HJPetBullet.h"
#include "Components/BoxComponent.h"

// Sets default values
AHJPetBullet::AHJPetBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 컴포넌트 생성 
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	// 루트 컴포넌트 지정 및 박스 크기 조정 
	SetRootComponent(compBox);
	compMesh->SetupAttachment(compBox);
	compBox->SetBoxExtent(FVector(10, 10, 10));

	// 메쉬 및 메테리얼 오브젝트 지정 // [BP] 원형 Mesh 스케일 조정 = 1-> 0.3 
	ConstructorHelpers::FObjectFinder<UStaticMesh>
		tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial>
		tempMat(TEXT("/Script/Engine.Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	if (tempMat.Succeeded())
	{
		compMesh->SetMaterial(0, tempMat.Object);
	}

}

// Called when the game starts or when spawned
void AHJPetBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJPetBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 총알 발사 방향 지정 
	FVector dirf = FVector::ForwardVector;
	FVector p0 = GetActorLocation();
	FVector vt = dirf * speed * DeltaTime;

	SetActorLocation(p0 + vt);

}

