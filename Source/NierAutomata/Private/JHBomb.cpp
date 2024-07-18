// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBomb.h"
#include "HJCharacter.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundAttenuation.h"

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
	//SphereComp->SetCollisionProfileName(TEXT("EnemySkill"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);	// EnemySkill
	SphereComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);	// Enemy

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
}

void AJHBomb::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// 부딪힌 대상이 플레이어라면 
	if (OtherActor->IsA<AHJCharacter>())
	{
		// 폭발 파티클을 생성하고 죽는다.
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PsExplosion.Particle, GetActorLocation(), FRotator::ZeroRotator, FVector(PsExplosion.Scale), true);
		this->Destroy();
	}
	// 만약 감지된 대상이 바닥이라면
	else if(OtherActor->GetName().Contains(TEXT("Destroy")))
	{
		// 폭발, 흔적 파티클을 재생하고 싶다.
		FVector Pos = GetActorLocation() * FVector(1, 1, 0);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PsExplosion.Particle, Pos, FRotator::ZeroRotator, FVector(PsExplosion.Scale), true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PsBlastMark.Particle, Pos, FRotator::ZeroRotator, FVector(PsBlastMark.Scale), true);
	}
	// 폭발음을 재생하고 싶다.
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation(), 1, 1, 0, ExplosionAttenuation);
}

// Called every frame
void AJHBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJHBomb::Fire()
{
	SphereComp->AddImpulse(GetActorUpVector() * Force);

}

void AJHBomb::SetForce(float Value)
{
	Force = Value;
}