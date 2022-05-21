// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelChange.generated.h"

UCLASS()
class HUNTERS_API ALevelChange : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelChange();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void LevelBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelProperties", meta = (AllowPrivateAccess = "true"))
	FName NewLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelProperties", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent *LevelChangeParticle;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelProperties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent *LevelBox;


};
