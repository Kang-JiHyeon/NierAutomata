// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBomb.h"
#include "Components/SphereComponent.h"

// Sets default values
AJHBomb::AJHBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(sphereComp);
	// 크기
	sphereComp->SetSphereRadius(50);
	// 물리
	sphereComp->SetSimulatePhysics(true);
	sphereComp->SetEnableGravity(true);
	sphereComp->SetMassOverrideInKg(NAME_None, 1);
	// 충돌체
	sphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	sphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	sphereComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);	// EnemySkill

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(sphereComp);
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));


	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSphere(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempSphere.Succeeded())
		meshComp->SetStaticMesh(tempSphere.Object);
	meshComp->SetRelativeLocation(FVector(0, 0, -50));

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("/Script/Engine.Material'/Engine/MapTemplates/Materials/BasicAsset01.BasicAsset01'"));
	if (tempMat.Succeeded())
		meshComp->SetMaterial(0, tempMat.Object);


}

// Called when the game starts or when spawned
void AJHBomb::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->AddImpulse(GetActorUpVector() * force);
	
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
	sphereComp->AddImpulse(GetActorUpVector() * force);

	//UE_LOG(LogTemp, Log, TEXT("폭탄 발사"));
}

//void AJHBomb::SetForce(float NewForce)
//{
//	force = NewForce;
//}
