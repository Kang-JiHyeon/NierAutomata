// Fill out your copyright notice in the Description page of Project Settings.


#include "HJPawn.h"
#include "vector"

// Sets default values
AHJPawn::AHJPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHJPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHJPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}