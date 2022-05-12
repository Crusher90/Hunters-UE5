// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

UEnemyAnimInstance::UEnemyAnimInstance() 
{
    
}

void UEnemyAnimInstance::UpdateAnimationProperties(float DeltaTime) 
{
    if(Enemy == nullptr)
    {
        Enemy = Cast<AEnemy>(TryGetPawnOwner());
    }
    if(Enemy)
    {
        FVector Velocity = Enemy->GetVelocity();
        Velocity.Z = 0.f;
        Speed = Velocity.Size();

        bIsInAir = Enemy->GetCharacterMovement()->IsFalling();

        if(Enemy->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
        {
            bIsAccelerating = true;
        }
        else
        {
            bIsAccelerating = false;
        }
    }
}

void UEnemyAnimInstance::NativeInitializeAnimation() 
{
    if(Enemy == nullptr)
    {
        Enemy = Cast<AEnemy>(TryGetPawnOwner());
    }
}


