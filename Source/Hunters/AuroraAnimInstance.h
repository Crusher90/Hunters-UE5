// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AuroraAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HUNTERS_API UAuroraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UAuroraAnimInstance();

	UFUNCTION(BlueprintCallable, Category = "AuroraAnim")
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AuroraAnim", meta = (AllowPrivateAccess = "true"))
	class AAurora *Aurora;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AuroraAnim", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AuroraAnim", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AuroraAnim", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
};
