// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JHBombSkill.generated.h"

UCLASS()
class NIERAUTOMATA_API AJHBombSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJHBombSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AJHBomb> BombFactory;

	UPROPERTY(EditAnywhere)
	TArray<class USceneComponent*> FirePositions;


	UPROPERTY(EditAnywhere)
	int BombCount = 10;

	float SkillTime = 1;
	float CurrTime = 0;

	float Force = 1000;
	int32 MaxFireCount = 3;
	int32 CurrFireCount = 0;


private:
	void BombSkill();


};
