// Fill out your copyright notice in the Description page of Project Settings.


#include "JHGuidedMissileSkill.h"
#include "JHGuidedMissile.h"

UJHGuidedMissileSkill::UJHGuidedMissileSkill()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UJHGuidedMissileSkill::BeginPlay()
{
	Super::BeginPlay();

	OnInitialize();
}

void UJHGuidedMissileSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bAttack)
	{
		if (SkillInfoByLevel.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("MissileSkillInfo가 없습니다."));
			return;
		}

		CurrFireTime += DeltaTime;

		if (CurrFireTime > SkillInfoByLevel[CurrSkillLevel].MaxFireTime)
		{
			Fire();
			CurrFireTime = 0;
			CurrFireCount++;
		}

		if (CurrFireCount >= SkillInfoByLevel[CurrSkillLevel].MaxFireCount)
		{
			OnEndAttack();
		}
	}
}

void UJHGuidedMissileSkill::OnInitialize()
{
	Super::OnInitialize();

	CurrFireCount = 0;
	CurrFireTime = 0;
}

void UJHGuidedMissileSkill::OnStartAttack()
{
	Super::OnStartAttack();
}

void UJHGuidedMissileSkill::OnEndAttack()
{
	Super::OnEndAttack();

	OnInitialize();
}

void UJHGuidedMissileSkill::Fire()
{
	int32 MaxMissilCount = SkillInfoByLevel[CurrSkillLevel].MaxMissileCount;

	for (int i = 0; i < MaxMissilCount; i++)
	{
		FRotator Rotation = FRotator(0, i * (360 / MaxMissilCount), 0);
		auto* Missile = GetWorld()->SpawnActor<AJHGuidedMissile>(GuidedMissileFactory, GetComponentLocation(), Rotation);

		if (Missile != nullptr)
		{
			Missile->SetActorLocation(Missile->GetActorLocation() + Missile->GetActorForwardVector() * Radius);
			Missile->OnExplosion();
			Missile->SetSpeed(SkillInfoByLevel[CurrSkillLevel].Speed);
		}
	}
}