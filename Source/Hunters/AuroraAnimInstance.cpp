// Fill out your copyright notice in the Description page of Project Settings.


#include "AuroraAnimInstance.h"
#include "Aurora.h"
#include "GameFramework/CharacterMovementComponent.h"

UAuroraAnimInstance::UAuroraAnimInstance()
{

}

void UAuroraAnimInstance::UpdateAnimationProperties(float DeltaTime) 
{
    if(Aurora == nullptr)
    {
        Aurora = Cast<AAurora>(TryGetPawnOwner());
    }
    if(Aurora)
    {
        FVector Velocity = Aurora->GetVelocity();
        Speed = Velocity.Size();

        bIsInAir = Aurora->GetCharacterMovement()->IsFalling();

        if(Aurora->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
        {
            bIsAccelerating = true;
        }
        else
        {
            bIsAccelerating = false;
        }
    }
}

void UAuroraAnimInstance::NativeInitializeAnimation() 
{
    if(Aurora == nullptr)
    {
        Aurora = Cast<AAurora>(TryGetPawnOwner());
    } 
}
