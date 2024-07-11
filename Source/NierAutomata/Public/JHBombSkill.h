// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "JHAttackInterface.h"
#include "JHBombSkill.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NIERAUTOMATA_API UJHBombSkill : public USceneComponent, public IJHAttackInterface
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

	virtual void OnAttack() override;

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AJHBomb> SkillFactory;

	UPROPERTY()
	TArray<class USceneComponent*> FirePositions;

	UPROPERTY(EditAnywhere)
	int BombCount = 10;

	UPROPERTY(EditAnywhere)
	TArray<float> Forces = {1000, 1200, 1400};
	int32 ForceIndex = 0;

	UPROPERTY(EditAnywhere)
	float SkillTime = 1;
	float CurrSkillTime = 0;

private:
	AActor* MyActor;

public:
	
	void Fire();
	void SetSkillTime(float Value);
};
