// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemySkillBase.h"
#include "JHEnemy.h"

// Sets default values for this component's properties
UJHEnemySkillBase::UJHEnemySkillBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJHEnemySkillBase::BeginPlay()
{
	Super::BeginPlay();

	// ...

	MyEnemy = Cast<AJHEnemy>(GetOwner());
	
}


// Called every frame
void UJHEnemySkillBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJHEnemySkillBase::OnInitialize()
{
}

void UJHEnemySkillBase::OnAttack()
{
}

void UJHEnemySkillBase::OnEnd()
{
	MyEnemy->OnEndAttackSkill();
	bAttack = false;
}

void UJHEnemySkillBase::SetCurrSkillLevel(ESkillLevel NewLevel)
{
	CurrSkillLevel = NewLevel;
}

