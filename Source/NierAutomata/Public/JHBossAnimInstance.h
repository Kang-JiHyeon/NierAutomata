// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JHEnemyFSM.h"
#include "JHBossAnimInstance.generated.h"

UCLASS()
class NIERAUTOMATA_API UJHBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	EEnemyState AnimState;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
    bool bAttackPlay = false;

};
