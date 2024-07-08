// Fill out your copyright notice in the Description page of Project Settings.


#include "HJJumpPet.h"

// Sets default values
AHJJumpPet::AHJJumpPet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ½ºÄÉ·¹Å» ¸Þ½Ã °´Ã¼ ºÎ¿© 
	JumpPet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("JumpPet"));
	RootComponent = JumpPet;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		tempPet(TEXT("/Script/Engine.SkeletalMesh'/Engine/EngineMeshes/SkeletalCube.SkeletalCube'"));
	if (tempPet.Succeeded())
	{
		JumpPet->SetSkeletalMesh(tempPet.Object);
	}

	FVector UpScale(3.0f, 3.0f, 3.0f);
	JumpPet->SetWorldScale3D(UpScale);
}

// Called when the game starts or when spawned
void AHJJumpPet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJJumpPet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

