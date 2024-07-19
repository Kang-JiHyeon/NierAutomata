// Fill out your copyright notice in the Description page of Project Settings.


#include "HJWeapon2.h"
#include "Components/BoxComponent.h"

// Sets default values
AHJWeapon2::AHJWeapon2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 콜리전 박스 설정
	CompBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	RootComponent = CompBox;
	CompBox->SetBoxExtent(FVector(8.f, 16.f, 140.f));

	// 스케레탈 메시 객체 부여 
	Weapon2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON2"));
	Weapon2->SetupAttachment(CompBox);

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		tempWeapon(TEXT("/Script/Engine.SkeletalMesh'/Game/Character/2B_Sword/2B_Sword4/claymore_LP_UVW.claymore_LP_UVW'"));
	if (tempWeapon.Succeeded())
	{
		Weapon2->SetSkeletalMesh(tempWeapon.Object);
	}



}

// Called when the game starts or when spawned
void AHJWeapon2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJWeapon2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

