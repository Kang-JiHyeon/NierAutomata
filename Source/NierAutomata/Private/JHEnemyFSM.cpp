// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemyFSM.h"

// Sets default values for this component's properties
UJHEnemyFSM::UJHEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJHEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UJHEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	

	//switch (mState)
	//{
	//	case EEnemyState::Idle:
	//		IdleState();
	//		break;
	//	case EEnemyState::Move:
	//		MoveState();
	//		break;
	//	case EEnemyState::Attack:
	//		AttackState();
	//		break;
	//	case EEnemyState::Damage:
	//		DamageState();
	//		break;
	//	case EEnemyState::Die:
	//		DieState();
	//		break;
	//	default:
	//		break;
	//}
}

void UJHEnemyFSM::IdleState()
{
	UE_LOG(LogTemp, Warning, TEXT("Idle!!"));
}

void UJHEnemyFSM::MoveState()
{
	UE_LOG(LogTemp, Warning, TEXT("Move!!"));
}

void UJHEnemyFSM::AttackState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack!!"));
}

void UJHEnemyFSM::DamageState()
{
	UE_LOG(LogTemp, Warning, TEXT("Damage!!"));
}

void UJHEnemyFSM::DieState()
{
	UE_LOG(LogTemp, Warning, TEXT("Die!!"));
}

