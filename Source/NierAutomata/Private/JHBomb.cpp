// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBomb.h"
#include "Components/SphereComponent.h"

// Sets default values
AJHBomb::AJHBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(50);
	sphereComp->SetSimulatePhysics(true);
	sphereComp->SetEnableGravity(true);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(sphereComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSphere(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempSphere.Succeeded()) {
		meshComp->SetStaticMesh(tempSphere.Object);
	}
	meshComp->SetRelativeLocation(FVector(0, 0, -50));


}

// Called when the game starts or when spawned
void AJHBomb::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->AddImpulse(FVector::UpVector);
	
}

// Called every frame
void AJHBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

