// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class HUNTERS_API AHealthPickup : public AItem
{
	GENERATED_BODY()
	
	public:
	AHealthPickup();

	virtual void ItemSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void ItemSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem *ItemParticle;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue *ItemSound;
};
