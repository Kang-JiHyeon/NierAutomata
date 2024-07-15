// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JHAttackInterface.h"
#include "JHEnemy.generated.h"

UCLASS()
class NIERAUTOMATA_API AJHEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJHEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


public:

	// Boss
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* RootCapsuleComp;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereTopComp;
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereBottomComp;

	// FSM
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UJHEnemyFSM* Fsm;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* PsDamageComp;

	//Manager
	UPROPERTY(EditAnywhere)
	class UJHBossSkillManager* BossSkillManager;


	 //SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* BottomSceneComp;

	class UMaterialInstance* BodyMatirial;

	UPROPERTY(EditAnywhere)
	class UJHBombSkill* BombSkill;

	UPROPERTY(EditAnywhere)
	class UJHMissileSkill* MissileSkill;

	UPROPERTY(EditAnywhere)
	class UJHLaserBeamSkill* LaserBeamSkill;
	
	UPROPERTY(EditAnywhere)
	class UJHSpiralMoveSkill* SpiralMoveSkill;

	// Arrow
	UPROPERTY(EditAnywhere)
	class UArrowComponent* SequentialMissileArrow;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* OnceMissileArrow;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJHEnemyDamageUI> DamageUIFactory;


private:
	// Move
	UPROPERTY(EditAnywhere)
	FVector MoveTargetPos;

	UPROPERTY(EditAnywhere)
	int32 MoveSpeed = 1000;

	// Rotate
	UPROPERTY(VisibleAnywhere)
	AActor* RotateTarget;

	UPROPERTY(VisibleAnywhere)
	FVector RotateTargetPos;

	FVector RotateTargetDirection;

	UPROPERTY(EditAnywhere)
	int32 RotSpeed = 400;

	UPROPERTY(EditAnywhere)
	int32 InterpSpeed = 1;

	// 제어 변수
	UPROPERTY(EditAnywhere)
	bool bIsMove;
	UPROPERTY(EditAnywhere)
	bool bIsRotatePosition;
	UPROPERTY(EditAnywhere)
	bool bIsLookAt;

public:
	void SetMovement(bool bValue);
	void SetLockAt(bool bValue);
	void SetMovePosition(FVector TargetPosotion);
	void SetRotSpeed(float Value);

	void Movement();
	void Movement(FVector TargetPosition);
	void RotateLookAt();
	//void RotateTarget(FVector TargetPosition);
	void RotateSpinBody();
	void RotateSpinBottom();

	void SetBodyMaterial(UMaterialInterface* NewMaterial);
	UMaterialInterface* GetBodyMaterial();
	
	UFUNCTION()
	void OnDamageProcess(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	void OnCreatedDamageUI(int32 Damage);
};
