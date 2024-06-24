// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HJAnimInstance2.generated.h"

/**
 * 
 */
UCLASS()
class NIERAUTOMATA_API UHJAnimInstance2 : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UHJAnimInstance2();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsInAir;
	// 공격 몽타주 연결 
	void PlayAttackMontage();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AttackMontage;


};
