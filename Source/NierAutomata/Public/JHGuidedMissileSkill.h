// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemySkillBase.h"
#include "JHGuidedMissileSkill.generated.h"

USTRUCT(Atomic)
struct FGuidedMissillInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	float MaxFireTime;
	UPROPERTY(EditAnywhere)
	float MaxFireCount;
	UPROPERTY(EditAnywhere)
	float MaxMissileCount;
	UPROPERTY(EditAnywhere)
	float Speed;
};


UCLASS()
class NIERAUTOMATA_API UJHGuidedMissileSkill : public UJHEnemySkillBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UJHGuidedMissileSkill();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnInitialize() override;
	virtual void OnStartAttack() override;
	virtual void OnEndAttack() override;

private:
	UPROPERTY(EditAnywhere)
	TMap<ESkillLevel, FGuidedMissillInfo> SkillInfoByLevel;

	// 미사일 공장
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AJHGuidedMissile> GuidedMissileFactory;

	UPROPERTY(EditAnywhere)
	float Radius = 300;

	float CurrFireTime = 0;
	float CurrFireCount = 0;

private:
	void Fire();
};
