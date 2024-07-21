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
		OnFire();

	}
}

void UJHMissileSkill::OnInitialize()
{
	Super::OnInitialize();

	CurrTime = 0;
	CurrFireCount = 0;
}

void UJHMissileSkill::OnStartAttack()
{
	Super::OnStartAttack();

	bAttack = true;
}

void UJHMissileSkill::OnEndAttack()
{
	Super::OnEndAttack();

	OnInitialize();
}

/// <summary>
/// ���� �ð����� �߻� 
/// </summary>
void UJHMissileSkill::OnFire()
{
	if(SkillInfoByLevel.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("MissileSkillInfo�� �����ϴ�."));
		return;
	}
	
	CurrTime += GetWorld()->DeltaTimeSeconds;

	if (CurrTime > SkillInfoByLevel[CurrSkillLevel].CreateTime) {
		
		if (MissileFactory == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("MissileFactory�� �����ϴ�."));
		}
		else
		{
			float RandX = FMath::RandRange(-RandomRotRange, RandomRotRange);
			float RandY = FMath::RandRange(-RandomRotRange, RandomRotRange);
			float RandZ = FMath::RandRange(-RandomRotRange, RandomRotRange);

			FRotator Rotation = FRotator(RandX, RandY, RandZ);

			GetWorld()->SpawnActor<AJHMissile>(MissileFactory, SkillArrow->GetComponentLocation(), Rotation);
		}

		CurrFireCount++;
		CurrTime = 0;
	}

	// �ִ� �߻� Ƚ����ŭ �߻������� ���� ����
	if (CurrFireCount >= SkillInfoByLevel[CurrSkillLevel].MaxFireCount)
	{
		OnEndAttack();
	}
}