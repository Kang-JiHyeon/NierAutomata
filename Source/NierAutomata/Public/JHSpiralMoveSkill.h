// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemySkillBase.h"
#include "JHSpiralMoveSkill.generated.h"

USTRUCT(Atomic)
struct FSpiralMoveSkillInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	float RadiusSpeed = 700;
	UPROPERTY(EditAnywhere)
	float AngleSpeed = 100;
	UPROPERTY(EditAnywhere)
	float MoveTime = 3;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHSpiralMoveSkill : public UJHEnemySkillBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJHSpiralMoveSkill();

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
	TMap<ESkillLevel, FSpiralMoveSkillInfo> SkillInfoByLevel;

	//UPROPERTY(EditAnywhere)
	//float RadiusSpeed = 700;

	//UPROPERTY(EditAnywhere)
	//float AngleSpeed = 100;

	//UPROPERTY(EditAnywhere)
	//float MoveTime = 3;

	UPROPERTY(VisibleAnywhere)
	float CurrTime = 0;

	UPROPERTY(EditAnywhere)
	USoundBase* AttackSound;

	FVector CenterPos;
	float Radius = 0;
	float DegreeAngle = 0;
	float Sign = 1;
	bool bStartAttack;


private:
	void SetCenterPosition(FVector Position);

};
