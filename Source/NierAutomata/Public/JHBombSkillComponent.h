// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemySkill.h"
#include "JHBombSkillComponent.generated.h"

/**
 * 
 */
UCLASS()
class NIERAUTOMATA_API UJHBombSkillComponent : public UJHEnemySkill
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UJHBombSkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	virtual void Attack() override;

public:
	
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AJHBomb> BombFactory;

	UPROPERTY(EditAnywhere)
	TArray<class USceneComponent*> FirePositions;


	UPROPERTY(EditAnywhere)
	int BombCount = 10;

	float SkillTime = 1;
	float CurrTime = 0;

	float Force = 1000;
	int32 MaxFireCount = 3;
	int32 CurrFireCount = 0;

};
