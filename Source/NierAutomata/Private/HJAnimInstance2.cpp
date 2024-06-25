// Fill out your copyright notice in the Description page of Project Settings.


#include "HJAnimInstance2.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UHJAnimInstance2::UHJAnimInstance2()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	// 공격 몽타주 구현
	ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK(TEXT("/Script/Engine.AnimMontage'/Game/Animation/HJAnimMontage.HJAnimMontage'"));
	if (ATTACK.Succeeded())
	{
		AttackMontage = ATTACK.Object;
	}
}

void UHJAnimInstance2::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UHJAnimInstance2::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}
// 콤보 공격 구현 
void UHJAnimInstance2::JumpToAttackMontageSection(int32 NewSection)
{
	FName name = GetAttackMontageSectionName(NewSection);
	UE_LOG(LogTemp, Warning, TEXT("What is the problem %s"), *name.ToString());
	Montage_JumpToSection(name, AttackMontage);
}

void UHJAnimInstance2::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UHJAnimInstance2::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UHJAnimInstance2::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attacked%d"), Section));
}

