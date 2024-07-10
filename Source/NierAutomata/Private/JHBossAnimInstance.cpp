// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBossAnimInstance.h"

//// 매 프레임마다 갱신되는 함수
//void UJHBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//    Super::NativeUpdateAnimation(DeltaSeconds);
//
//    // 플레이어의 이동 속도 가져오기
//    
//    AActor* OwnerActor = GetOwningActor();
//    AJHEnemy* Boss = Cast<AJHEnemy>(OwnerActor);
//
//    if (Boss)
//    {
//        FVector Velocity = Boss->GetVelocity();
//
//        Speed = Velocity.Length();
//        //FVector ForwardVector = Boss->GetActorForwardVector();
//        //Speed = FVector::DotProduct(ForwardVector, Velocity);
//
//        UE_LOG(LogTemp, Warning, TEXT("Speed : %f"), Speed);
//    }
//}
