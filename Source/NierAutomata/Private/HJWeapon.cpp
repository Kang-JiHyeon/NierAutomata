// Fill out your copyright notice in the Description page of Project Settings.


#include "HJWeapon.h"

// Sets default values
AHJWeapon::AHJWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// �ݸ��� �ڽ� ����
	CompBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	RootComponent = CompBox;
	CompBox->SetBoxExtent(FVector(8.f, 16.f, 100.f));


	// ���ɷ�Ż �޽� ��ü �ο� 
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	Weapon->SetupAttachment(CompBox);

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		tempWeapon(TEXT("/Script/Engine.SkeletalMesh'/Game/Mesh/SK_Blade_BlackKnight.SK_Blade_BlackKnight'"));
	if (tempWeapon.Succeeded())
	{
		Weapon->SetSkeletalMesh(tempWeapon.Object);
	}

}

// Called when the game starts or when spawned
void AHJWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

