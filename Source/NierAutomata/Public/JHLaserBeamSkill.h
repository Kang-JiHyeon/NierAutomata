// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "JHAttackInterface.h"
#include "JHLaserBeamSkill.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHLaserBeamSkill : public USceneComponent, public IJHAttackInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJHLaserBeamSkill();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnInitialize() override;

	virtual void OnAttack() override;

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AJHLaserBeam> SkillFactory;

	UPROPERTY(EditAnywhere)
	float Radius = 1000;

	UPROPERTY(EditAnywhere)
	int32 MaxCount = 10; 

	UPROPERTY(EditAnywhere)
	float RotSpeed = 100;

	TArray<AJHLaserBeam*> LaserBeams;

	bool bEnable;

private:
	void ToggleEnableActor();
	FVector GetPositionOnCircle(float Radius, float Angle, FVector CenterPos);
};
