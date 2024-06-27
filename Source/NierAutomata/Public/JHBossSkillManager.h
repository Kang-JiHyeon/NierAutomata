// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JHBossSkillManager.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Bomb,
	Missile,
	LaserBeam
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHBossSkillManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJHBossSkillManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:

	UPROPERTY()
	class AJHEnemy* Me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType MSkillType = ESkillType::Bomb;

	UPROPERTY(EditAnywhere)
	TArray<class IJHAttackInterface*> AttackSkills;

};
