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

	//UPROPERTY(EditAnywhere)
	//class USceneComponent* SuperFirePos;

	UPROPERTY()
	class AJHEnemy* Me;

	UPROPERTY()
	TSubclassOf<class AJHBomb> BombFactory;

	UPROPERTY()
	TArray<class USceneComponent*> FirePositions;

	UPROPERTY(EditAnywhere)
	int BombCount = 10;

	UPROPERTY(EditAnywhere)
	TArray<float> Forces = {700, 1000, 1200};
	int32 ForceIndex = 0;

	UPROPERTY(EditAnywhere)
	float SkillTime = 1;
	float CurrSkillTime = 0;

	int32 MaxFireCount = 3;
	int32 CurrFireCount = 0;


public:
	UFUNCTION()
	void Attack();

	UFUNCTION()
	void Fire();

	void SetSkillTime(float Value);


};
