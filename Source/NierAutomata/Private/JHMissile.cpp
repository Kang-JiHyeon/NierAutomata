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
	// ũ��
	BoxComp->SetBoxExtent(FVector(50, 50, 50));
	BoxComp->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));

	// �浹ü
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);	// EnemySkill

	// �Ž�
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
	
	// �÷��̾ ã�� Ÿ������ �����Ѵ�.
	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	Direction = FVector::ForwardVector;
}

// Called every frame
void AJHMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� �ð� ������ ������ �̵��ϴٰ�

	if (CurrUpTime <= UpTime) {
		CurrUpTime += DeltaTime;
	}
	else
	{
		// ���� �ð����ȸ� Ÿ������ �̵�
		if (CurrTraceTime <= TraceTime)
		{
			CurrTraceTime += DeltaTime;
			
			// todo : pos z 0���� �����ϱ�!!

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


	// todo : �̻��� �浿 ó�� �ʿ�!!
	Destroy();
}
