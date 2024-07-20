// Fill out your copyright notice in the Description page of Project Settings.


#include "JHLaserBeam.h"
#include "HJCharacter.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"

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

	// Niagara System
	NSLaser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NSLaser"));
	NSLaser->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraLaserFinder(TEXT("/Script/Niagara.NiagaraSystem'/Game/Assets/Kang/NS_Laser.NS_Laser'"));
	if (NiagaraLaserFinder.Succeeded())
	{
		NSLaser->SetAsset(NiagaraLaserFinder.Object);
	}

	NSLaserImpact = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NSLaserImpact"));
	NSLaserImpact->SetupAttachment(RootComponent);
	
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraLaserImpactFinder(TEXT("/Script/Niagara.NiagaraSystem'/Game/Assets/Kang/NS_LaserImpact.NS_LaserImpact'"));
	if (NiagaraLaserImpactFinder.Succeeded())
	{
		NSLaserImpact->SetAsset(NiagaraLaserImpactFinder.Object);
	}

	// Sound
	LaserAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("LaserAudioComp"));
	LaserAudioComp->SetupAttachment(RootComponent);
	
	LaserEffectAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("LaserEffectAudioComp"));
	LaserEffectAudioComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AJHLaserBeam::BeginPlay()
{
	Super::BeginPlay();

	// Hit 대상 설정
	AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	// ray Ignore
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(Player);

	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	//ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);

	ObjectParams.RemoveObjectTypesToQuery(ECollisionChannel::ECC_Camera);
	ObjectParams.RemoveObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel3);
	ObjectParams.RemoveObjectTypesToQuery(ECollisionChannel::ECC_Visibility);

	// Beam 상태 Idle 상태로 변경
	SetLaserBeamState(ELaserBeamState::Idle);
	LaserAudioComp->Play();

	// Niagara 설정
	NSLaser->SetVisibility(false);
	NSLaserImpact->SetVisibility(false);

	NSLaser->SetColorParameter(FName(TEXT("Color")), FLinearColor::Red);
	NSLaserImpact->SetColorParameter(FName(TEXT("Color")), FLinearColor::Red);

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

		// Hit Location
		HitLocation = Hit.Location;
		bHit = true;
	}
	else
	{
		bHit = false;
	}

	// 일정 시간이 지나면 대기 모드에서 공격 모드로 전환
	if (CurrIdleTime <= IdleTime) {
		CurrIdleTime += DeltaTime;
	}
	else
	{
		SetLaserBeamState(ELaserBeamState::Attack);

		FVector LaserEnd = bHit ? HitLocation : Forward;
		NSLaser->SetVectorParameter(TEXT("LaserEnd"), LaserEnd);
		NSLaser->SetVisibility(true);

		NSLaserImpact->SetWorldLocation(StartPos);
		NSLaserImpact->SetVisibility(true);

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

	bool bIdle = State == ELaserBeamState::Idle;

	// StaticMesh, Material 변경
	FLaserBeamInfo LaserInfo = bIdle ? IdleStyle : AttackStyle;

	SplineMesh->SetStaticMesh(LaserInfo.StaticMesh);
	SplineMesh->SetMaterial(0, LaserInfo.Material);

	// Scale 변경
	FVector2D TargetScale = FVector2D(LaserInfo.Scale);
	SplineMesh->SetStartScale(TargetScale);
	SplineMesh->SetEndScale(TargetScale);

	// Laser마다 다른 Sound로 교체
	LaserAudioComp->SetSound(LaserInfo.Sound);

	// 노출 활성화 여부 설정
	SplineMesh->SetVisibility(bIdle);

	// 대기 상태일 때
	if (bIdle)
	{
		// Collision 비활성화
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	// 공격 상태일 때
	else
	{
		// Collision 활성화
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// Effect 재생
		UGameplayStatics::PlaySound2D(GetWorld(), EffectSound, 0.1f);
	}
	

}
