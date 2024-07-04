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

	UPROPERTY(VisibleAnywhere)
	APawn* Target;

	UPROPERTY(VisibleAnywhere)
	FVector MovePosition;

	UPROPERTY()
	FVector Direction;

	UPROPERTY(EditAnywhere)
	float Speed = 1500;

	UPROPERTY(EditAnywhere)
	float UpTime = 1;

	UPROPERTY() 
	float CurrUpTime;

	UPROPERTY(EditAnywhere)
	float TraceTime = 2;

	UPROPERTY()
	float CurrTraceTime;


	UPROPERTY()
	bool bTrace;

};
