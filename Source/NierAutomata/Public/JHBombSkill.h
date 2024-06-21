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
	class UCapsuleComponent* capsuleComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AJHBomb> bombFactory;

	//UPROPERTY(EditAnywhere)
	//class USceneComponent* firePosisions;

	UPROPERTY(EditAnywhere)
	TArray<class USceneComponent*> firePositions;


	UPROPERTY(EditAnywhere)
	int bombCount = 10;



};
