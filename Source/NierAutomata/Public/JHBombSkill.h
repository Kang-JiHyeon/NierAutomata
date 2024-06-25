// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "JHBombSkill.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NIERAUTOMATA_API UJHBombSkill : public USceneComponent
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

public:

	UPROPERTY(EditAnywhere)
	class USceneComponent* SuperFirePos;

	UPROPERTY(EditAnywhere)
	class AJHEnemy* Me;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AJHBomb> BombFactory;

	UPROPERTY(EditAnywhere)
	TArray<class USceneComponent*> FirePositions;


	UPROPERTY(EditAnywhere)
	int BombCount = 10;

	float SkillTime = 2;
	float CurrTime = 0;

	float Force = 1000;
	int32 MaxFireCount = 3;
	int32 CurrFireCount = 0;

	void Attack();
	void Fire();
};
