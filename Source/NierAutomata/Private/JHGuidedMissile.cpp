// Fill out your copyright notice in the Description page of Project Settings.


#include "JHGuidedMissile.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include <HJCharacter.h>

// Sets default values
AJHGuidedMissile::AJHGuidedMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	// 크기
	BoxComp->SetBoxExtent(FVector(50, 50, 50));
	BoxComp->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));

	// 충돌체
	BoxComp->SetCollisionProfileName(TEXT("EnemySkill"));

	// 매쉬
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/ControlRig/Controls/ControlRig_Wedge_solid.ControlRig_Wedge_solid'"));
	if (TempMesh.Succeeded()) {
		MeshComp->SetStaticMesh(TempMesh.Object);
	}

	// Particle
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleFinder(TEXT("/Script/Engine.ParticleSystem'/Game/Effects/Effects/FX_Monsters/FX_Monster_Gruntling/Master/P_BackPack_MuzzleFlash.P_BackPack_MuzzleFlash'"));
	if (ParticleFinder.Succeeded())
	{
		PsExplosion = ParticleFinder.Object;
	}
}

// Called when the game starts or when spawned
void AJHGuidedMissile::BeginPlay()
{
	Super::BeginPlay();
	
	Direction = FVector::UpVector;

	Target = GetWorld()->GetFirstPlayerController()->GetPawn();
	bRotate = true; 
}

// Called every frame
void AJHGuidedMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//if (bRotate)
	//	return;
	
	// 일정 시간 동안 위의 방향으로 회전하고 싶다.
	// - UpTime
	// 일정 시간이 지나면 플레이어의 방향으로 회전하고 싶다.
	// - Player
	// 플레이어 방향으로 회전한지 일정 시간이 지나면
	// - TraceTime
	// 현재 방향 그대로 이동하고 싶다.
	// - Dir
	// 플레이어랑 일정 거리 이하로 가까워졌다면
	// 더이상 회전하지 않는다.

	if (CurrUpTime <= UpTime)
	{
		CurrUpTime += DeltaTime;

		TargetRotator = GetActorRotation() + UKismetMathLibrary::MakeRotFromX(FVector::UpVector);
	}
	else
	{       
		if (CurrTraceTime <= TraceTime)
		{
			CurrTraceTime += DeltaTime;
			
			Direction = Target->GetActorLocation() - GetActorLocation();

			if (Direction.Length() > 50 && bRotate)
			{
				TargetRotator = UKismetMathLibrary::Conv_VectorToRotator(Direction);
			}
			else
			{
				bRotate = false;
			}
		}
	}

	if (bRotate)
	{
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotator, DeltaTime, 5);
		SetActorRotation(NewRotation);
	}

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);
}

void AJHGuidedMissile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	OnExplosion();

	if (OtherActor->IsA<AHJCharacter>())
	{
		this->Destroy();
	}
}

void AJHGuidedMissile::OnExplosion()
{
	if (PsExplosion != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PsExplosion, GetActorLocation(), FRotator(90, 0, 0), true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation(), 0.5f, 1, 0, Attenuation);

		if (CameraShake != nullptr)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraShake);
		}
	}
}

void AJHGuidedMissile::SetSpeed(float Value)
{
	Speed = Value;
}
