// Fill out your copyright notice in the Description page of Project Settings.


#include "JHLaserBeam.h"
#include "HJCharacter.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AJHLaserBeam::AJHLaserBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMesh"));
	SplineMesh->SetForwardAxis(ESplineMeshAxis::Z);

	// Init
	ConstructorHelpers::FObjectFinder<UStaticMesh> InitMeshFinder(TEXT(""));
	if (InitMeshFinder.Succeeded()) {
		IdleStyle.StaticMesh = InitMeshFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterial> InitMatFinder(TEXT(""));
	if (InitMatFinder.Succeeded()) {
		IdleStyle.Material = InitMatFinder.Object;
	}

	// Active
	ConstructorHelpers::FObjectFinder<UStaticMesh> ActiveMeshFinder(TEXT(""));
	if (ActiveMeshFinder.Succeeded()) {
		AttackStyle.StaticMesh = ActiveMeshFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterial> ActiveMatFinder(TEXT(""));
	if (ActiveMatFinder.Succeeded()) {
		AttackStyle.Material = ActiveMatFinder.Object;
	}
}

// Called when the game starts or when spawned
void AJHLaserBeam::BeginPlay()
{
	Super::BeginPlay();
	
	// Spline Mesh
	UpdateBeamStyle(&IdleStyle);
	
	//SplineMesh->SetStaticMesh(InitMesh);
	//SplineMesh->SetMaterial(0, InitMat);

	// ray
	Params.AddIgnoredActor(GetOwner());

	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel3);

}

// Called every frame
void AJHLaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrIdleTime <= IdleDelayTime) {
		CurrIdleTime += DeltaTime;
	}
	else {
		UpdateBeamStyle(&AttackStyle);
		AttackState();
	}
}

void AJHLaserBeam::SetIdleDelayTime(float Value)
{
	IdleDelayTime = Value;
}

void AJHLaserBeam::SetCurrIdleTime(float Value)
{
	CurrIdleTime = Value;
}

void AJHLaserBeam::AttackState()
{
	// Laycast로 Hit한 대상까지 발사하고 싶다.
	StartPos = GetActorLocation();
	EndPos = StartPos + GetActorForwardVector() * Distance;

	// Hit 대상이 있을 경우
	if (GetWorld()->LineTraceSingleByObjectType(Hit, StartPos, EndPos, ObjectParams, Params))
	{
		// todo : 플레이어 감지 처리
		AHJCharacter* Player = Cast<AHJCharacter>(Hit.GetActor());

		if (Player != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("LaserBeam과 Player가 충돌!"));
		}

		UE_LOG(LogTemp, Warning, TEXT("LaserBeam과 Hit한 대상 : %s"), *Hit.GetActor()->GetName());

		DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Green, false, 0.01f, 0, 5.0f);
	}
	// Hit 대상이 없을 경우
	else
	{
		DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, 0.01f, 0, 5.0f);
	}
}

void AJHLaserBeam::UpdateBeamStyle(FLaserBeamStyle* Style)
{
	SplineMesh->SetStaticMesh(Style->StaticMesh);
	SplineMesh->SetMaterial(0, Style->Material);

	SplineMesh->SetStartAndEnd(FVector::ZeroVector, FVector(100, 0, 0), FVector::ForwardVector * Distance, FVector(100, 0, 0), true);
	
	FVector2D TargetScale = FVector2D(Style->Scale.Y, Style->Scale.Z);

	SplineMesh->SetStartScale(TargetScale);
	SplineMesh->SetEndScale(TargetScale);
}