// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "JHRotation.generated.h"

UENUM(BlueprintType)
enum class ERotateState : uint8
{
	None,
	Target,
	LockAt
};

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	Actor,
	SceneComponent
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIERAUTOMATA_API UJHRotation : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJHRotation();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
public:
	UPROPERTY(EditAnywhere)
    ETargetType TargetType = ETargetType::Actor;

	UPROPERTY(EditAnywhere)
	ERotateState RotateState = ERotateState::None;

private:

	UPROPERTY(EditAnywhere)
	AActor * TargetOwner;

	UPROPERTY(EditAnywhere)
	USceneComponent* TargetSceneComp;

	UPROPERTY(EditAnywhere)
	float Speed;

	void RotateLockAt();

};
