// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemySkillBase.h"
#include "JHMissileSkill.generated.h"

USTRUCT(Atomic)
struct FMissilSkillInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	float CreateTime = 0.1f;
	UPROPERTY(EditAnywhere)
	int32 MaxFireCount = 10;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHMissileSkill : public UJHEnemySkillBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJHMissileSkill();

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
	TMap<ESkillLevel, FMissilSkillInfo> SkillInfoByLevel;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class AJHMissile> MissileFactory;

    UPROPERTY()
	class UArrowComponent* SkillArrow;

	UPROPERTY(EditAnywhere)
	float RandomRotRange = 30;

	UPROPERTY(EditAnywhere)
	float Radius = 200;

	float CurrTime;
	int32 CurrFireCount = 0;

private:
	void OnFire();
};
