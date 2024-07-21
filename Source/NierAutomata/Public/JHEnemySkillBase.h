// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "JHEnemySkillBase.generated.h"

UENUM(BlueprintType)
enum class ESkillLevel : uint8
{
	Easy,
	Hard
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHEnemySkillBase : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJHEnemySkillBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY()
	class AJHEnemy* MyEnemy;

	UPROPERTY(EditAnywhere)
	ESkillLevel CurrSkillLevel = ESkillLevel::Easy;

	UPROPERTY(VisibleAnywhere)
	bool bAttack;

	virtual void OnInitialize();
	virtual void OnStartAttack();
	virtual void OnEndAttack();

	void SetCurrSkillLevel(ESkillLevel TargetLevel);
};
