// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemySkill.h"
#include "JHGuidedMissile.generated.h"

UCLASS()
class NIERAUTOMATA_API AJHGuidedMissile : public AJHEnemySkill
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJHGuidedMissile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;



	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* PsExplosion;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere)
	class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
	class USoundAttenuation* Attenuation;

	UPROPERTY(EditAnywhere)
	AActor* Target;

	UPROPERTY(EditAnywhere)
	float UpTime = 1;
	float CurrUpTime = 0;

	UPROPERTY(EditAnywhere)
	float TraceTime = 5;
	float CurrTraceTime = 0;

	UPROPERTY(EditAnywhere)
	float Speed = 3000;

	FVector Direction;
	FRotator TargetRotator;
	bool bRotate;

public:
	void OnExplosion();
	void SetSpeed(float Value);

};
