// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class HUNTERS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Private Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent *AgroCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent *AttackCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent *EnemySwordCollision1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent *EnemySwordCollision2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class AAIController *AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	bool bOverlappingAttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage *EnemyAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class AAurora *AuroraCombatTarget;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem *BloodParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class USoundCue *BladeSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	float EnemyMaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	float EnemyHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class AWeapon *Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem *EnemyDestroyParticles;

public:
	// Public Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	float DamageToAurora;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageTypeClass2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem *EnemyBloodParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	class USoundCue *EnemyHitSound;

	FTimerHandle DisappearTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	bool bOverlappingAgroCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent *EnemyHealthBar;

	// Public Functions
	UFUNCTION()
	virtual void AgroCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void AgroCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void AttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void AttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void Sword1CollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void Sword1CollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void Sword2CollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void Sword2CollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateSword1Collision();

	UFUNCTION(BlueprintCallable)
	void DeactivateSword1Collision();

	UFUNCTION(BlueprintCallable)
	void ActivateSword2Collision();

	UFUNCTION(BlueprintCallable)
	void DeactivateSword2Collision();

	UFUNCTION(BlueprintCallable)
	void SpawnFX();

	void MoveToTarget(class AAurora *Target);

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	void Die();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	void Disappear();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;
};
