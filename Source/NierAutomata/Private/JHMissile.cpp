// Fill out your copyright notice in the Description page of Project Settings.


#include "JHMissile.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);	// EnemySkill

	// 매쉬
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (TempMesh.Succeeded()) {
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	//MeshComp->SetRelativeLocation(FVector(0, 0, -50));

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
	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	Direction = FVector::ForwardVector;
}

// Called every frame
void AJHMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 일정 시간 동안은 앞으로 이동하다가

	if (CurrUpTime <= UpTime) {
		CurrUpTime += DeltaTime;
	}
	else
	{
		// 일정 시간동안만 타겟으로 이동
		if (CurrTraceTime <= TraceTime)
		{
			CurrTraceTime += DeltaTime;
			
			// todo : pos z 0으로 설정하기!!

			FVector pos = Target->GetActorLocation();

			Direction = Target->GetActorLocation() - GetActorLocation();
			Direction.Normalize();
			
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
			SetActorRotation(Rotator);
		}
	}

	SetActorLocation(GetActorLocation() + Direction * Speed * DeltaTime);
}

void AJHMissile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);


	// todo : 미사일 충동 처리 필요!!
	Destroy();
}
