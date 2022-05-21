// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChange.h"
#include "Components/BoxComponent.h"
#include "Aurora.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ALevelChange::ALevelChange()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	LevelBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LevelBox"));
	SetRootComponent(LevelBox);

	LevelChangeParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LevelChangeParticles"));
	LevelChangeParticle->SetupAttachment(GetRootComponent());

	NewLevelName = "Map1";
}

// Called when the game starts or when spawned
void ALevelChange::BeginPlay()
{
	Super::BeginPlay();

	LevelBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelChange::LevelBoxOverlapBegin);
}

// Called every frame
void ALevelChange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelChange::LevelBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if(OtherActor)
	{
		class AAurora *Aurora = Cast<AAurora>(OtherActor);
		if(Aurora)
		{
			Aurora->SwitchLevel(NewLevelName);
		}
	}
}


