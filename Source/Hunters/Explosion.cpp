// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Aurora.h"

AExplosion::AExplosion()
{
    // default values
    ExplosiveDamage = 20.f;
}


void AExplosion::ItemSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    Super::ItemSphereOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if(OtherActor)
    {
        Aurora = Cast<AAurora>(OtherActor);
        if(Aurora)
        {
            if(ItemSound)
            {
                UGameplayStatics::PlaySound2D(this, ItemSound);
            }
            if(ItemParticle)
            {
                UGameplayStatics::SpawnEmitterAtLocation(this, ItemParticle, GetActorLocation(), FRotator(0.f));
            }
            if(DamageTypeClass)
            {
                UGameplayStatics::ApplyDamage(Aurora, ExplosiveDamage, nullptr, this, DamageTypeClass);
            }
            Destroy();
        }
    }
}

void AExplosion::ItemSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
    Super::ItemSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
  