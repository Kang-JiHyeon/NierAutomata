// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemySkillBase.h"
#include "JHBombSkill.generated.h"

USTRUCT(Atomic)
struct FBombSkillInfo
{
	GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere)
	int MaxFireCount = 5;
    UPROPERTY(EditAnywhere)
	float FireTime = 1;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NIERAUTOMATA_API UJHBombSkill : public UJHEnemySkillBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UJHBombSkill();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnInitialize() override;
	virtual void OnStartAttack() override;
	virtual void OnEndAttack() override;

public:
	UPROPERTY(EditAnywhere)
	TMap<ESkillLevel, FBombSkillInfo> SkillInfoByLevel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AJHBomb> SkillFactory;

	UPROPERTY()
	TArray<class USceneComponent*> FirePositions;


	// 한번에 발사할 폭탄 개수
	UPROPERTY(EditAnywhere)
	int BombCount = 10;

	// 발사 힘
	UPROPERTY(EditAnywhere)
	TArray<float> Forces = {1000, 1200, 1400};
	int32 ForceIndex = 0;

	// 발사 횟수
	int CurrFireCount = 0;
	// 발사 주기
	float CurrFireTime = 0;

public:


	void Fire();
};
