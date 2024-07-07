// Fill out your copyright notice in the Description page of Project Settings.


#include "JHMissileSkill.h"
#include "Components/ArrowComponent.h"
#include "JHEnemy.h"
#include "JHMissile.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UJHMissileSkill::UJHMissileSkill()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UJHMissileSkill::BeginPlay()
{
	Super::BeginPlay();

	CurrTime = 0;
	bIsAttack = false;	
}


// Called every frame
void UJHMissileSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UJHMissileSkill::OnInitialize()
{
	CurrTime = 0;
	bIsAttack = false;

	if (MissileSpawnType == EMissileSpawnType::Sequential)
		MissileSpawnType = EMissileSpawnType::AtOnce;
	else
		MissileSpawnType = EMissileSpawnType::Sequential;
}

void UJHMissileSkill::OnAttack()
{

	switch (MissileSpawnType)
	{
	case EMissileSpawnType::Sequential:
		OnSpawnSequential();
		break;
	case EMissileSpawnType::AtOnce:
		OnSpawnAtOnce();
		break;
	default:
		break;
	}
}

/// <summary>
/// 일정 시간마다 발사 
/// </summary>
void UJHMissileSkill::OnSpawnSequential()
{
	CurrTime += GetWorld()->DeltaTimeSeconds;

	if (CurrTime > CreateTime) {
		CurrTime = 0;

		if (SequentialMissileFactory == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("MissileFactory가 없습니다."))
		}
		else
		{
			float RandX = FMath::RandRange(-RandomRotRange, RandomRotRange);
			float RandY = FMath::RandRange(-RandomRotRange, RandomRotRange);
			float RandZ = FMath::RandRange(-RandomRotRange, RandomRotRange);

			FRotator Rotation = FRotator(RandX, RandY, RandZ);

			GetWorld()->SpawnActor<AJHMissile>(SequentialMissileFactory, SequentilSkillArrow->GetComponentLocation(), Rotation);
		}
	}
}

/// <summary>
/// n발을 동시에 한번만 발사
/// </summary>
void UJHMissileSkill::OnSpawnAtOnce()
{
	if (!bIsAttack)
	{
		for (int i = 0; i < MaxCount; i++)
		{
			FRotator Rotation = FRotator(0, i * (360 / MaxCount), 0);

			AJHMissile* Missile = GetWorld()->SpawnActor<AJHMissile>(OnceMissileFactory, OnceSkillArrow->GetComponentLocation(), Rotation);

			if (Missile != nullptr)
			{
				Missile->SetActorRelativeLocation(Missile->GetActorForwardVector() * Radius);
			}
		}

		bIsAttack = true;
	}
}