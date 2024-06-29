// Fill out your copyright notice in the Description page of Project Settings.


#include "JHMissileSkill.h"
#include "JHEnemy.h"
#include "JHMissile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"

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

}


// Called every frame
void UJHMissileSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UJHMissileSkill::OnInitialize()
{
	CurrTime = 0;
}

void UJHMissileSkill::OnAttack()
{
	CurrTime += GetWorld()->DeltaTimeSeconds;

	if (CurrTime > CreateTime) {
		CurrTime = 0;

		// todo: 랜덤 회전값 부여

		// 미사일 생성
		//AJHMissile* Missile = GetWorld()->SpawnActor<AJHMissile>(SkillFactory, SkillPosition->GetComponentLocation(), SkillPosition->GetRelativeRotation());
		AJHMissile* Missile = GetWorld()->SpawnActor<AJHMissile>(SkillFactory, SkillPosition->GetComponentLocation(), SkillPosition->GetComponentRotation());
	}
}
