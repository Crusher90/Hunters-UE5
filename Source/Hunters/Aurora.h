// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Aurora.generated.h"

UCLASS()
class HUNTERS_API AAurora : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAurora();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Protected Functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Private Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent *CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	float LookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	float LookRightRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	class AWeapon *EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage *AuroraCombatMontage;

public:
	// Public Functions
	void FKeyPressed();
	void GKeyPressed();
	void Attack();
	void EKeyPressed();
	void ShiftKeyPressed();
	void ShiftKeyReleased();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

	void IncrementCoin();

	void IncrementHealth();

	UFUNCTION(BlueprintCallable)
	void ActivateBladeCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateBladeCollision();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	void Die();

	void SwitchLevel(FName LevelName);

	void OpenLevelWhenDie();

	// void DestroyAurora();

	FTimerHandle DestroyTimer;

	// bool bESCDown;
	// void ESCDown();
	// void ESCUp();

	// Public Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	bool bHasWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	float AuroraMaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	float AuroraHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	float AuroraMaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	float AuroraStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	int32 Coins;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	bool bSprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	class USoundCue *WeaponEquipSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	class AAuroraPlayerController *AuroraController;

	FTimerHandle OpenLevelTimer;

public:
	// Getters
	FORCEINLINE USpringArmComponent *GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent *GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE AWeapon *GetEquippedWeapon() const {return EquippedWeapon;}

	// Setters
	FORCEINLINE void SetEquippedWeapon(AWeapon *Weapon) { EquippedWeapon = Weapon; }
};
