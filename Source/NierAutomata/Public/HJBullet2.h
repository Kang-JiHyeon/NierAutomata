// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HJBullet2.generated.h"

UCLASS()
class NIERAUTOMATA_API AHJBullet2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJBullet2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*void FireLaser(FVector& StartPoint, FVector& EndPoint);*/

	/*UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* LaserBeamPSC;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* LaserBeamEffect;*/
	
	//// ������ ������Ʈ 
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class USplineComponent* Laser;

	//UPROPERTY(EditAnywhere)
	//UStaticMesh* LaserMesh;

	//UPROPERTY(EditAnywhere)
	//UMaterialInterface* LaserMaterial;

	//UPROPERTY(EditAnywhere)
	//TArray<class USplineMeshComponent*> SplineMeshComponents;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float SegmentLength;

	//// ������ �߻� �Լ� 
	//UPROPERTY(EditAnywhere)
	//void FireLaser(FVector& StartPoint, FVector& EndPoint);

	/*UPROPERTY(EditAnywhere)
	void UpdateSplineMesh();*/

};
