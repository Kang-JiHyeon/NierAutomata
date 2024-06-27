// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "JHMissileSkill.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHMissileSkill : public USceneComponent
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

	//virtual void Attack() override;


public:

	UPROPERTY()
	class AJHEnemy* Me;

	UPROPERTY()
	TSubclassOf<class AJHMissile> SkillFactory;

	UPROPERTY()
	class UArrowComponent* SkillPosition;

	UPROPERTY(EditAnywhere)
	float CreateTime = 0.25f;
	
	UPROPERTY(EditAnywhere)
	int32 CreateCount = 10;

	UPROPERTY()
	int32 CurrCreateCount;

	UPROPERTY()
	float CurrTime;

	UPROPERTY(EditAnywhere)
	FVector Offset;
		


	void Attack();

};
