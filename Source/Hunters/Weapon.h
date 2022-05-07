// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class HUNTERS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Protected Functions
	void WeaponAfterDrop();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Private Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	class AAurora *Aurora;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent *WeaponBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent *WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	float LinearDamping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	float AngularDamping;

	FTimerHandle WeaponTimer;

	float RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	bool bShouldRotateWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperties", meta = (AllowPrivateAccess = "true"))
	float DamageToEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageTypeClass;

public:
	// Public Functions
	UFUNCTION()
	void WeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void WeaponBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void WeaponBladeBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void WeaponBladeBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	void Equip();

	void Drop();

	void ActivateBladeCollision();
	void DeactivateBladeCollision();

	// Public Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent *WeaponBladeBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent *WeaponWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	FString WeaponName;

public:
	// Getters
	FORCEINLINE UBoxComponent *GetWeaponBox() const { return WeaponBox; }
	FORCEINLINE USkeletalMeshComponent *GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE AAurora *GetAurora() const { return Aurora; }

	// Setters
};
