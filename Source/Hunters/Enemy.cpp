// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Aurora.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "AuroraPlayerController.h"
#include "Components/WidgetComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AgroCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AgroCollision"));
	AgroCollision->SetupAttachment(GetRootComponent());
	AgroCollision->InitSphereRadius(1000.f);

	AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetRootComponent());
	AttackCollision->InitSphereRadius(100.f);

	EnemySwordCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Swordbox1"));
	EnemySwordCollision1->SetupAttachment(GetMesh(), FName("LeftHandSocket"));

	EnemySwordCollision2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Swordbox2"));
	EnemySwordCollision2->SetupAttachment(GetMesh(), FName("RightHandSocket"));

	EnemyHealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	EnemyHealthBar->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->MaxWalkSpeed = 350.f;

	// Default Values
	bAttacking = false;
	bOverlappingAttackCollision = false;
	bOverlappingAgroCollision = false;
	DamageToAurora = 10.f;
	EnemyMaxHealth = 100.f;
	EnemyHealth = 100.f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());

	AgroCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroCollisionOverlapBegin);
	AgroCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroCollisionOverlapEnd);
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackCollisionOverlapBegin);
	AttackCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AttackCollisionOverlapEnd);

	EnemySwordCollision1->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::Sword1CollisionOverlapBegin);
	EnemySwordCollision1->OnComponentEndOverlap.AddDynamic(this, &AEnemy::Sword1CollisionOverlapEnd);
	EnemySwordCollision2->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::Sword2CollisionOverlapBegin);
	EnemySwordCollision2->OnComponentEndOverlap.AddDynamic(this, &AEnemy::Sword2CollisionOverlapEnd);

	EnemySwordCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemySwordCollision2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	EnemyHealthBar->SetVisibility(false);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AgroCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if(OtherActor)
	{
		AAurora *Aurora = Cast<AAurora>(OtherActor);
		if(Aurora)
		{
			EnemyHealthBar->SetVisibility(true);
			bOverlappingAgroCollision = true;
			MoveToTarget(Aurora);
		}
	}
}

void AEnemy::AgroCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if(OtherActor)
	{
		AAurora *Aurora = Cast<AAurora>(OtherActor);
		if(Aurora)
		{
			EnemyHealthBar->SetVisibility(false);
			bOverlappingAgroCollision = false;
			if(AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}

void AEnemy::AttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if(OtherActor)
	{
		AAurora *Aurora = Cast<AAurora>(OtherActor);
		if(Aurora)
		{
			bOverlappingAttackCollision = true;
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, 0.5);
		}
	}
}

void AEnemy::AttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if(OtherActor)
	{
		AAurora *Aurora = Cast<AAurora>(OtherActor);
		if(Aurora)
		{
			bOverlappingAttackCollision = false;
			MoveToTarget(Aurora);
		}
	}
}

void AEnemy::Sword1CollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if(OtherActor)
	{
		AAurora *Aurora = Cast<AAurora>(OtherActor);
		if(Aurora)
		{
			if(DamageTypeClass && bAttacking)
			{
				const USkeletalMeshSocket *WeaponSocket1 = GetMesh()->GetSocketByName(FName("WeaponTipSocket_L"));
				FVector WeaponSocket1Location = WeaponSocket1->GetSocketLocation(GetMesh());
				if(BloodParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticles, WeaponSocket1Location, FRotator(0.f));
				}
				UGameplayStatics::ApplyDamage(Aurora, DamageToAurora, AIController, this, DamageTypeClass);
			}
		}
	}
}

void AEnemy::Sword1CollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	
}

void AEnemy::Sword2CollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if(OtherActor)
	{
		AAurora *Aurora = Cast<AAurora>(OtherActor);
		if(Aurora)
		{
			if(DamageTypeClass && bAttacking)
			{
				const USkeletalMeshSocket *WeaponSocket1 = GetMesh()->GetSocketByName(FName("WeaponTipSocket_L"));
				FVector WeaponSocket1Location = WeaponSocket1->GetSocketLocation(GetMesh());
				if(BloodParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticles, WeaponSocket1Location, FRotator(0.f));
				}
				UGameplayStatics::ApplyDamage(Aurora, DamageToAurora, AIController, this, DamageTypeClass2);
			}
		}
	}
}

void AEnemy::Sword2CollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	
}

void AEnemy::ActivateSword1Collision() 
{
	EnemySwordCollision1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateSword1Collision() 
{
	EnemySwordCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::ActivateSword2Collision() 
{
	EnemySwordCollision1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateSword2Collision() 
{
	EnemySwordCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::SpawnFX()
{
	if(BladeSound)
	{
		UGameplayStatics::PlaySound2D(this, BladeSound);
	}
}

void AEnemy::MoveToTarget(AAurora *Target) 
{
	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.0f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
	}	
}

void AEnemy::Attack() 
{
	if(!bAttacking)
	{
		bAttacking = true;
		UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance)
		{
			int32 Section = FMath::RandRange(0, 2);
			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(EnemyAttackMontage);
				AnimInstance->Montage_JumpToSection(FName("Attack_A"));
				break;
			case 1:
				AnimInstance->Montage_Play(EnemyAttackMontage);
				AnimInstance->Montage_JumpToSection(FName("Attack_B"));
				break;
			case 2:
				AnimInstance->Montage_Play(EnemyAttackMontage);
				AnimInstance->Montage_JumpToSection(FName("Attack_C"));
				break;
			}
		}
	}
}

void AEnemy::AttackEnd() 
{
	bAttacking = false;
	if(bOverlappingAttackCollision)
	{
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, 0.5);
	}
}

void AEnemy::Die() 
{
	class UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		int32 Section = FMath::RandRange(0, 1);
		switch (Section)
		{
		case 0:
			AnimInstance->Montage_Play(EnemyAttackMontage);
			AnimInstance->Montage_JumpToSection(FName("Death_A"));
			break;
		
		case 1:
			AnimInstance->Montage_Play(EnemyAttackMontage);
			AnimInstance->Montage_JumpToSection(FName("Death_B"));
			break;			
		}
	}
	AgroCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemySwordCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemySwordCollision2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bOverlappingAttackCollision = false;
	bAttacking = false;

	GetWorldTimerManager().SetTimer(DisappearTimer, this, &AEnemy::Disappear, 1.f);
}

void AEnemy::DeathEnd() 
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AEnemy::Disappear()
{
	Destroy();
	if(EnemyDestroyParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EnemyDestroyParticles, GetActorLocation(), FRotator(0.f));
	}
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) 
{
	if(EnemyHealth - DamageAmount <= 0.f)
	{
		EnemyHealth = 0;
		Die();
	}
	else
	{
		EnemyHealth -= DamageAmount;
	}
	return DamageAmount;
}

