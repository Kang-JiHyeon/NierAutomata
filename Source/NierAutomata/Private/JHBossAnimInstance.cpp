// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBossAnimInstance.h"

//// �� �����Ӹ��� ���ŵǴ� �Լ�
//void UJHBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//    Super::NativeUpdateAnimation(DeltaSeconds);
//
//    // �÷��̾��� �̵� �ӵ� ��������
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
