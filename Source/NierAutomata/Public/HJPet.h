// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HJPet.generated.h"

UCLASS()
class NIERAUTOMATA_API AHJPet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJPet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* firePos;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJPetBullet> bulletFactory;


	float speed = 500;
	// 타겟 엑터 지정 
	UPROPERTY(EditAnywhere)
	class AActor* target;

	float currTime = 0;

	UPROPERTY(EditAnywhere)
	float createTime = 1;

	// 카메라 이동 
	void Turn(float value);
	void Lookup(float value);

};
