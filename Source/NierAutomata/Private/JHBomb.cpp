// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBomb.h"
#include "Components/SphereComponent.h"

// Sets default values
AJHBomb::AJHBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(SphereComp);
	// 크기
	SphereComp->SetSphereRadius(50);
	// 물리
	SphereComp->SetSimulatePhysics(true);
	SphereComp->SetEnableGravity(true);
	SphereComp->SetMassOverrideInKg(NAME_None, 1);
	// 충돌체
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);	// EnemySkill

	// 매쉬
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(SphereComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));


	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSphere(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempSphere.Succeeded())
		MeshComp->SetStaticMesh(tempSphere.Object);
	MeshComp->SetRelativeLocation(FVector(0, 0, -50));

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("/Script/Engine.Material'/Engine/MapTemplates/Materials/BasicAsset01.BasicAsset01'"));
	if (tempMat.Succeeded())
		MeshComp->SetMaterial(0, tempMat.Object);


}

// Called when the game starts or when spawned
void AJHBomb::BeginPlay()
{
	Super::BeginPlay();

	//sphereComp->AddImpulse(GetActorUpVector() * force);
	
}

// Called every frame
void AJHBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJHBomb::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	Destroy();
}

void AJHBomb::Fire()
{
	SphereComp->AddImpulse(GetActorUpVector() * Force);

	//UE_LOG(LogTemp, Log, TEXT("폭탄 발사"));
}

void AJHBomb::SetForce(float Value)
{
	Force = Value;
}

