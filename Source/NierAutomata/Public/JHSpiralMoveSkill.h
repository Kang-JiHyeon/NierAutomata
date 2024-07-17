// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JHAttackInterface.h"
#include "JHSpiralMoveSkill.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHSpiralMoveSkill : public UActorComponent, public IJHAttackInterface
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

	virtual void OnAttack() override;

private:

	// 각도
	UPROPERTY(VisibleAnywhere)
	float Radius;

	UPROPERTY(EditAnywhere)
	float RadiusSpeed = 700;

	UPROPERTY(VisibleAnywhere)
	float DegreeAngle;

	UPROPERTY(EditAnywhere)
	float AngleSpeed = 100;

	// 시간
	UPROPERTY(EditAnywhere)
	float MoveTime = 3;

	UPROPERTY(VisibleAnywhere)
	float CurrTime;

	UPROPERTY(VisibleAnywhere)
	float Sign = 1;

	UPROPERTY(EditAnywhere)
	bool bStartAttack;

	UPROPERTY(EditAnywhere)
	USoundBase* AttackSound;

	class AJHEnemy* MyOwner;
	FVector CenterPos;


public:
	void SetCenterPosition(FVector Position);

};
