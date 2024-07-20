// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemySkillBase.h"
#include "JHLaserBeamSkill.generated.h"

USTRUCT(Atomic)
struct FLaserBeamSkillInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	float IdleTime = 3;
	UPROPERTY(EditAnywhere)
	float AttackTime = 10;
	UPROPERTY(EditAnywhere)
	int32 MaxCount = 10;
	UPROPERTY(EditAnywhere)
	float RotSpeed = 20;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHLaserBeamSkill : public UJHEnemySkillBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJHLaserBeamSkill();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnInitialize() override;
	virtual void OnAttack() override;
	virtual void OnEnd() override;

public:

	UPROPERTY(EditAnywhere)
	TMap<ESkillLevel, FLaserBeamSkillInfo> SkillInfoByLevel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AJHLaserBeam> SkillFactory;

	UPROPERTY(EditAnywhere)
	float Radius = 350;


	UPROPERTY()
	TArray<class AJHLaserBeam*> LaserBeams;

	bool bRotate;

	FTimerHandle IdleTimerHandle;
	FTimerHandle AttackTimerHandle;

private:
	
	void CreateLaserBeams();
	void DestroyLaserBeams();

	void SetActiveAttack();
};
