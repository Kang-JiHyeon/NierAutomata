// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemySkill.h"
#include "JHBomb.generated.h"

USTRUCT(Atomic)
struct FBombParticle
{
	GENERATED_USTRUCT_BODY()
public :
	UPROPERTY(EditAnywhere)
	class UParticleSystem* Particle;
	UPROPERTY(EditAnywhere)
	float Scale;
};

UCLASS()
class NIERAUTOMATA_API AJHBomb : public AJHEnemySkill
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJHBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere)
	float Force = 1000;

	// Particle System
	UPROPERTY(EditAnywhere)
	FBombParticle PsExplosion;
	UPROPERTY(EditAnywhere)
	FBombParticle PsBlastMark;

	// Sound
	UPROPERTY(EditAnywhere)
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
	USoundAttenuation* ExplosionAttenuation;


public:
	void Fire();
	void SetForce(float Value);
};
