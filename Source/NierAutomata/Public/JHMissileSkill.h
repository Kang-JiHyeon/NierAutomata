// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemySkillBase.h"
#include "JHMissileSkill.generated.h"

UENUM(BlueprintType)
enum class EMissileSpawnType : uint8
{
	Sequential,
	AtOnce,
};
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
	virtual void OnAttack() override;
	virtual void OnEndAttack() override;

public:

	EMissileSpawnType MissileSpawnType = EMissileSpawnType::Sequential;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class AJHMissile> SequentialMissileFactory;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class AJHMissile> OnceMissileFactory;

    UPROPERTY()
	class UArrowComponent* SequentilSkillArrow;

    UPROPERTY()
	class UArrowComponent* OnceSkillArrow;

	UPROPERTY(EditAnywhere)
	TMap<ESkillLevel, FMissilSkillInfo> MissileSkillInfo;

	UPROPERTY(EditAnywhere)
	float RandomRotRange = 30;

	UPROPERTY(EditAnywhere)
	//float CreateTime = 0.25f;
	float CurrTime;

	int32 CurrFireCount = 0;
	//UPROPERTY(EditAnywhere)
	//int32 MaxFireCount = 10;
	//int32 CurrCount = 0;

	UPROPERTY(EditAnywhere)
	float Radius = 200;
	
	//bool bIsAttack;
	//bool bIsSequential;


private:
	void OnSpawnSequential();
	void OnSpawnAtOnce();
};
