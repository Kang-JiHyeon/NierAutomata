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
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}
}

