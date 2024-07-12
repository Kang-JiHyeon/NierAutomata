// Fill out your copyright notice in the Description page of Project Settings.


#include "JHLaserBeam.h"
#include "HJCharacter.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AJHLaserBeam::AJHLaserBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	SplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMesh"));
	SplineMesh->SetForwardAxis(ESplineMeshAxis::Z);
	SplineMesh->SetupAttachment(RootComponent);

	// Idle
	ConstructorHelpers::FObjectFinder<UStaticMesh> IdleMeshFinder(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (IdleMeshFinder.Succeeded()) {
		IdleStyle.StaticMesh = IdleMeshFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterial> InitMatFinder(TEXT("/Script/Engine.Material'/Engine/EditorLandscapeResources/LayerVisMaterial.LayerVisMaterial'"));
	if (InitMatFinder.Succeeded()) {
		IdleStyle.Material = InitMatFinder.Object;
	}

	// 
	ConstructorHelpers::FObjectFinder<UStaticMesh> AttackMeshFinder(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (AttackMeshFinder.Succeeded()) {
		AttackStyle.StaticMesh = AttackMeshFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterial> ActiveMatFinder(TEXT("/Script/Engine.Material'/Engine/VREditor/LaserPointer/LaserPointerMaterial.LaserPointerMaterial'"));
	if (ActiveMatFinder.Succeeded())
	{
		AttackStyle.Material = ActiveMatFinder.Object;
	}


	// ParticleSystem
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemFinder(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/Kang/P_JHLaser_Red.P_JHLaser_Red'"));
	if (ParticleSystemFinder.Succeeded())
	{
		ParticleSystem->SetTemplate(ParticleSystemFinder.Object);
	}
}

// Called when the game starts or when spawned
void AJHLaserBeam::BeginPlay()
{
	Super::BeginPlay();

	// Hit 대상 설정
	AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	// ray
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(Player);

	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);

	ObjectParams.RemoveObjectTypesToQuery(ECollisionChannel::ECC_Camera);
	ObjectParams.RemoveObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel3);

	// Beam 상태 Idle 상태로 변경
	SetLaserBeamState(ELaserBeamState::Idle);


	UE_LOG(LogTemp, Warning, TEXT("LaserBeam 생성 위치 : (%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
}

// Called every frame
void AJHLaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StartPos = GetActorLocation();
	Forward = StartPos + GetActorForwardVector() * Distance;

	// Hit 대상까지 Mesh 길이 변경
	if (GetWorld()->LineTraceSingleByObjectType(Hit, StartPos, Forward, ObjectParams, Params))
	{
		HitDistance = (Hit.Location - StartPos).Length();

		// SlineMesh 길이 변경
		SplineMesh->SetStartAndEnd(FVector::ZeroVector, FVector(100, 0, 0), FVector::ForwardVector * HitDistance, FVector(100, 0, 0), true);
	}

	// 일정 시간이 지나면 대기 모드에서 공격 모드로 전환
	if (CurrIdleTime <= IdleTime) {
		CurrIdleTime += DeltaTime;
	}
	else {
		SetLaserBeamState(ELaserBeamState::Attack);
	}
}

void AJHLaserBeam::SetIdleTime(float Value)
{
	IdleTime = Value;
}

/// <summary>
/// Beam의 상태를 변경하는 함수
/// </summary>
/// <param name="State"></param>
void AJHLaserBeam::SetLaserBeamState(ELaserBeamState State)
{
	if (CurLaserBeamState == State) return;

	// 현재 state 업데이트
	CurLaserBeamState = State;

	// collision 활성화 여부 설정
	bool bIsIdle = State == ELaserBeamState::Idle;
	SplineMesh->SetGenerateOverlapEvents(!bIsIdle);

	// StaticMesh, Material 변경
	FLaserBeamStyle Style = bIsIdle ? IdleStyle : AttackStyle;

	SplineMesh->SetStaticMesh(Style.StaticMesh);
	SplineMesh->SetMaterial(0, Style.Material);

	FVector2D TargetScale = FVector2D(Style.Scale.Y, Style.Scale.Z);
	SplineMesh->SetStartScale(TargetScale);
	SplineMesh->SetEndScale(TargetScale);
}
