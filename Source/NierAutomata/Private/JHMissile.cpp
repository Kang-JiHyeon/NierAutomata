// Fill out your copyright notice in the Description page of Project Settings.


#include "JHMissile.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include <HJCharacter.h>

// Sets default values
AJHMissile::AJHMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(BoxComp);
	// 크기
	BoxComp->SetBoxExtent(FVector(50, 50, 50));
	BoxComp->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));

	// 충돌체
	BoxComp->SetCollisionProfileName(TEXT("EnemySkill"));

	// 매쉬
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (TempMesh.Succeeded()) {
		MeshComp->SetStaticMesh(TempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> TempMat(TEXT("/Script/Engine.Material'/Engine/MapTemplates/Materials/BasicAsset01.BasicAsset01'"));
	if (TempMat.Succeeded()) {
		MeshComp->SetMaterial(0, TempMat.Object);
	}

}

// Called when the game starts or when spawned
void AJHMissile::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이어를 찾아 타겟으로 설정한다.
	Target = GetWorld()->GetFirstPlayerController()->GetPawn();
	TargetPosition = GetActorLocation() + (FVector::UpVector * OffsetUp + GetActorForwardVector() * OffsetForward);
}

// Called every frame
void AJHMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missile의 Target이 null입니다."));
		return;
	}

	// 위쪽 방향으로 회전하다가 일정 시간이 지나면 타겟 방향으로 회전하고 싶다.
	if (CurrTime <= TraceDelayTime)
	{
		CurrTime += DeltaTime;
		Direction = TargetPosition - GetActorLocation();
	}
	else
	{
		TargetPosition = Target->GetActorLocation();
		TargetPosition.Z = 0;

		Direction = TargetPosition - GetActorLocation();
	}

	// 현재 방향과 타겟 방향 사이의 회전 값 계산
	Direction.Normalize();
	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = Direction.Rotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);

	// 미사일 회전 설정
	SetActorRotation(NewRotation);

	// 미사일 위치 이동
	FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * MoveSpeed * DeltaTime);
	SetActorLocation(NewLocation);
}

void AJHMissile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA<AHJCharacter>())
	{
		Destroy();
	}
}
