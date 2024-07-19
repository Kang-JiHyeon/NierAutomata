// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HJWeapon2.generated.h"

UCLASS()
class NIERAUTOMATA_API AHJWeapon2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJWeapon2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 무기 매쉬 
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Weapon2;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CompBox;

};
