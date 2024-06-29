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

	// Line
	UPROPERTY(EditAnywhere)
	class USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* LineMesh;

	UPROPERTY(EditAnywhere)
	class UMaterial* LineMaterial;

	UPROPERTY(EditAnywhere)
	int32 MaxCount = 10; 

	UPROPERTY(EditAnywhere)
	float Distance = 5000;


	FVector StartPos;
	FVector EndPos;
	FVector Forward;

	FHitResult Hit;
	FCollisionQueryParams Params;
	FCollisionObjectQueryParams ObjectParams;

private:
	FVector GetPositionOnCircle(float Radius, float Angle, FVector CenterPos);

	void CreateLine();
};
