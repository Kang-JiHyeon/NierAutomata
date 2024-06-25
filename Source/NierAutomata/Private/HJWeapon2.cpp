// Fill out your copyright notice in the Description page of Project Settings.


#include "HJWeapon2.h"

// Sets default values
AHJWeapon2::AHJWeapon2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON2"));
	RootComponent = Weapon2;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		tempWeapon2(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_GreatBlade/SK_Blade_GreatBlade.SK_Blade_GreatBlade'"));
	if (tempWeapon2.Succeeded())
	{
		Weapon2->SetSkeletalMesh(tempWeapon2.Object);
	}

	Weapon2->SetCollisionProfileName(TEXT("NoCollision"));

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

