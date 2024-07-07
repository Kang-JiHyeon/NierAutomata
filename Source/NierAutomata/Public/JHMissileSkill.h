// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "JHAttackInterface.h"
#include "JHMissileSkill.generated.h"

UENUM(BlueprintType)
enum class EMissileSpawnType : uint8
{
	Sequential,
	AtOnce,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHMissileSkill : public USceneComponent, public IJHAttackInterface
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

	virtual void OnInitialize() override;

	virtual void OnAttack() override;

public:

	EMissileSpawnType MissileSpawnType = EMissileSpawnType::Sequential;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AJHMissile> SequentialMissileFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AJHMissile> OnceMissileFactory;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* SequentilSkillArrow;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* OnceSkillArrow;

	UPROPERTY(EditAnywhere)
	float RandomRotRange = 30;

	UPROPERTY(EditAnywhere)
	float CreateTime = 0.25f;
	float CurrTime;

	UPROPERTY(EditAnywhere)
	int32 MaxCount = 10;

	UPROPERTY(EditAnywhere)
	float Radius = 200;
	
	bool bIsAttack;
	bool bIsSequential;


private:
	void OnSpawnSequential();
	void OnSpawnAtOnce();
};
