// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemySkill.h"
#include "JHLaserBeam.generated.h"

UENUM(BlueprintType)
enum class ELaserBeamState : uint8
{
	None,
	Idle,
	Attack,
};

USTRUCT(Atomic)
struct FLaserBeamStyle
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	class UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere)
	class UMaterial* Material;
	UPROPERTY(EditAnywhere)
	FVector Scale;
};

UCLASS()
class NIERAUTOMATA_API AJHLaserBeam : public AJHEnemySkill
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJHLaserBeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleAnywhere)
	ELaserBeamState CurLaserBeamStyle = ELaserBeamState::None;

	UPROPERTY(EditAnywhere)
	class USplineMeshComponent* SplineMesh;

	UPROPERTY(EditAnywhere)
	FLaserBeamStyle IdleStyle;

	UPROPERTY(EditAnywhere)
	FLaserBeamStyle AttackStyle;

	UPROPERTY(EditAnywhere)
	float Distance = 10000;

private:
	UPROPERTY(EditAnywhere)
	float IdleTime = 3;

	float CurrIdleTime = 0;

	FVector StartPos;
	FVector EndPos;

	FHitResult Hit;
	FCollisionQueryParams Params;
	FCollisionObjectQueryParams ObjectParams;

public :
	void SetIdleTime(float Value);
	void SetCurrIdleTime(float Value);
	void SetLaserBeamState(ELaserBeamState State);

private:

	void OnAttack();
	void SetStyle(FLaserBeamStyle* Style);

};
