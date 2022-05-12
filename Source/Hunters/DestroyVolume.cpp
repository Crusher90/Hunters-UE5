// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyVolume.h"
#include "Components/BoxComponent.h"
#include "Aurora.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADestroyVolume::ADestroyVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DestroyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DestroyVolume"));
	SetRootComponent(DestroyCollision);
}

// Called when the game starts or when spawned
void ADestroyVolume::BeginPlay()
{
	Super::BeginPlay();

	DestroyCollision->OnComponentBeginOverlap.AddDynamic(this, &ADestroyVolume::DestroyBoxBeginOverlap);
}

// Called every frame
void ADestroyVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestroyVolume::DestroyBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if(OtherActor)
	{
		class AAurora *Aurora = Cast<AAurora>(OtherActor);
		if(Aurora)
		{
			Aurora->Die();
			// GetWorldTimerManager().SetTimer(NewLevelTimer, this, &ADestroyVolume::OpenNewLevel, 2.f);
		}
	}
}

// void ADestroyVolume::OpenNewLevel() 
// {
// 	UGameplayStatics::OpenLevel(GetWorld(), FName("ElvenRuins"));
// }



