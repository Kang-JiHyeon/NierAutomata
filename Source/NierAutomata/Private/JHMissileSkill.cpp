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
}


// Called every frame
void UJHMissileSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bAttack)
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
}

void UJHMissileSkill::OnInitialize()
{
	Super::OnInitialize();

	CurrTime = 0;
	//bIsAttack = false;
	bAttack = false;

	//if (MissileSpawnType == EMissileSpawnType::Sequential)
	//	MissileSpawnType = EMissileSpawnType::AtOnce;
	//else
	//	MissileSpawnType = EMissileSpawnType::Sequential;
}

void UJHMissileSkill::OnAttack()
{
	Super::OnAttack();

	bAttack = true;
}

void UJHMissileSkill::OnEnd()
{
	Super::OnEnd();

	CurrFireCount = 0;
}

/// <summary>
/// ���� �ð����� �߻� 
/// </summary>
void UJHMissileSkill::OnSpawnSequential()
{
	if(SkillInfoByLevel.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("MissileSkillInfo�� �����ϴ�."));
		return;
	}
	
	CurrTime += GetWorld()->DeltaTimeSeconds;

	if (CurrTime > SkillInfoByLevel[CurrSkillLevel].CreateTime) {
		
		if (SequentialMissileFactory == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("MissileFactory�� �����ϴ�."));
		}
		else
		{
			float RandX = FMath::RandRange(-RandomRotRange, RandomRotRange);
			float RandY = FMath::RandRange(-RandomRotRange, RandomRotRange);
			float RandZ = FMath::RandRange(-RandomRotRange, RandomRotRange);

			FRotator Rotation = FRotator(RandX, RandY, RandZ);

			GetWorld()->SpawnActor<AJHMissile>(SequentialMissileFactory, SequentilSkillArrow->GetComponentLocation(), Rotation);
		}

		CurrFireCount++;
		CurrTime = 0;
	}

	// �ִ� �߻� Ƚ����ŭ �߻������� ���� ����
	if (CurrFireCount >= SkillInfoByLevel[CurrSkillLevel].MaxFireCount)
	{
		OnEnd();
	}
}

/// <summary>
/// n���� ���ÿ� �ѹ��� �߻�
/// </summary>
void UJHMissileSkill::OnSpawnAtOnce()
{
	if (SkillInfoByLevel.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("MissileSkillInfo�� �����ϴ�."));
		return;
	}

	for (int i = 0; i < SkillInfoByLevel[CurrSkillLevel].MaxFireCount; i++)
	{
		FRotator Rotation = FRotator(0, i * (360 / SkillInfoByLevel[CurrSkillLevel].MaxFireCount), 0);

		//AJHMissile* Missile = GetWorld()->SpawnActor<AJHMissile>(OnceMissileFactory, OnceSkillArrow->GetComponentLocation() + Missile->GetActorForwardVector() * Radius, Rotation);
		AJHMissile* Missile = GetWorld()->SpawnActor<AJHMissile>(OnceMissileFactory, OnceSkillArrow->GetComponentLocation(), Rotation);

		if (Missile != nullptr)
		{
			Missile->SetActorLocation(Missile->GetActorLocation() + Missile->GetActorForwardVector() * Radius);
		}
	}
}