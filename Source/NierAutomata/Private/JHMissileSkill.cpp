// Fill out your copyright notice in the Description page of Project Settings.


#include "JHMissileSkill.h"
#include "JHEnemy.h"
#include "JHMissile.h"

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

	// 일정 시간마다 미사일을 N개 만든다.
	Me = Cast<AJHEnemy>(GetOwner());

	SkillFactory = Me->MessileFactory;
	SkillPosition = Me->MessilePosition;

}


// Called every frame
void UJHMissileSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



}

void UJHMissileSkill::Attack()
{
	CurrTime += GetWorld()->DeltaTimeSeconds;

	if (CurrTime > CreateTime) {
		CurrTime = 0;

		AJHMissile* Missile = GetWorld()->SpawnActor<AJHMissile>(SkillFactory, SkillPosition->GetComponentLocation(), SkillPosition->GetComponentRotation());
	}
}