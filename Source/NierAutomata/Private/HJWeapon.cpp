// Fill out your copyright notice in the Description page of Project Settings.


#include "HJWeapon.h"

// Sets default values
AHJWeapon::AHJWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// ½ºÄÉ·¹Å» ¸Þ½Ã °´Ã¼ ºÎ¿© 
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		tempWeapon(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
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

