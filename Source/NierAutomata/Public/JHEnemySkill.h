// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JHEnemySkill.generated.h"

UCLASS()
class NIERAUTOMATA_API AJHEnemySkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJHEnemySkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	int DamageAmount = 10;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int GetDamage();

};
