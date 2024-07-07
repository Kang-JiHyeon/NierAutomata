// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemySkill.h"
#include "JHMissile.generated.h"

UCLASS()
class NIERAUTOMATA_API AJHMissile : public AJHEnemySkill
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJHMissile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

private:
	
	UPROPERTY(VisibleAnywhere)
	APawn* Target;

	UPROPERTY(EditAnywhere)
	float OffsetForward;

	UPROPERTY(EditAnywhere)
	float OffsetUp;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 2000;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 5;

	UPROPERTY(EditAnywhere)
	float TraceDelayTime = 0.5f;

	FVector TargetPosition;
	FVector Direction;
	float CurrTime;
};
