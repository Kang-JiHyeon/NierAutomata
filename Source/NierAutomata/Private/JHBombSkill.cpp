// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBombSkill.h"
#include "JHBomb.h"
#include "JHEnemy.h"

// Sets default values for this component's properties
UJHBombSkill::UJHBombSkill()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UJHBombSkill::BeginPlay()
{
	Super::BeginPlay();

	MyActor = GetOwner();
}

// Called every frame
void UJHBombSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UJHBombSkill::Fire()
{

	if (FirePositions.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("FirePositions null!!"));
		return;
	}

	if (SkillFactory == nullptr) {

		UE_LOG(LogTemp, Warning, TEXT("BombFactory null!!"));
		return;
	}

	for (auto FirePos : FirePositions)
	{
		if (FirePos == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("FirePositions[i] null!!"));
			return;
		}

		AJHBomb* Bomb = GetWorld()->SpawnActor<AJHBomb>(SkillFactory, FirePos->GetComponentLocation(), FirePos->GetComponentRotation());

		if (Bomb != nullptr)
		{
 			Bomb->SetForce(Forces[ForceIndex]);
			Bomb->Fire();
			UE_LOG(LogTemp, Warning, TEXT("Bottom Fire!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Bottom Null!"));
		}
	}

	ForceIndex = ++ForceIndex % Forces.Num();
}

void UJHBombSkill::SetSkillTime(float Value)
{
	SkillTime = Value;
}

void UJHBombSkill::OnInitialize()
{
	CurrSkillTime = 0;
	ForceIndex = 0;
}

void UJHBombSkill::OnAttack()
{
	CurrSkillTime += GetWorld()->DeltaTimeSeconds;

	if (CurrSkillTime >= SkillTime) {

		CurrSkillTime = 0;
		Fire();
	}
}