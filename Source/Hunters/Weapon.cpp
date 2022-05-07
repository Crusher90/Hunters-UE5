// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Aurora.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/WidgetComponent.h"

// Sets default values
AWeapon::AWeapon():
	WeaponName(FString("Default"))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	SetRootComponent(WeaponBox);
	WeaponBox->InitBoxExtent(FVector(70.f));

	WeaponBladeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBladeBox"));
	WeaponBladeBox->SetupAttachment(GetRootComponent());

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(WeaponBox);

	WeaponWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WeaponWidget"));
	WeaponWidget->SetupAttachment(GetRootComponent());

	// default Values
	LinearDamping = 4.f;
	AngularDamping = 4.f;
	RotationRate = 45.f;
	bShouldRotateWeapon = true;
	DamageToEnemy = 20.f;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::WeaponBoxBeginOverlap);
	WeaponBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::WeaponBoxEndOverlap);
	WeaponBox->SetSimulatePhysics(false);
	WeaponBox->SetEnableGravity(false);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	WeaponBladeBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponBladeBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	WeaponBladeBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::WeaponBladeBoxBeginOverlap);
	WeaponBladeBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::WeaponBladeBoxEndOverlap);

	WeaponWidget->SetVisibility(false);
}


// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bShouldRotateWeapon)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);  
	}
}

void AWeapon::WeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if(OtherActor)
	{
		Aurora = Cast<AAurora>(OtherActor);
		if(Aurora)
		{
			WeaponWidget->SetVisibility(true);
			if(Aurora->bHasWeapon)
			{
				
			}
			else
			{
				Aurora->SetEquippedWeapon(this);
			}
		}
	}
}

void AWeapon::WeaponBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) 
{
	if(OtherActor)
	{
		Aurora = Cast<AAurora>(OtherActor);
		if(Aurora)
		{
			WeaponWidget->SetVisibility(false);
			if(Aurora->bHasWeapon)
			{

			}
			else
			{
				Aurora->SetEquippedWeapon(nullptr);
			}
		}
	}
}

void AWeapon::WeaponBladeBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if(OtherActor)
	{
		class AEnemy *Enemy = Cast<AEnemy>(OtherActor);
		if(Enemy)
		{
			const USkeletalMeshSocket *WeaponTipSocket = WeaponMesh->GetSocketByName(FName("WeaponTipSocket"));
			if(WeaponTipSocket)
			{
				FVector WeaponTipSocketLocation = WeaponTipSocket->GetSocketLocation(WeaponMesh);
				if(Enemy->EnemyBloodParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->EnemyBloodParticles, WeaponTipSocketLocation, FRotator(0.f));
				}
				if(Enemy->EnemyHitSound)
				{
					UGameplayStatics::PlaySound2D(this, Enemy->EnemyHitSound);
				}
			}
			if(DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Enemy, DamageToEnemy, nullptr, this, DamageTypeClass);
			}
		}
	}
}

void AWeapon::WeaponBladeBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) 
{
	
}

void AWeapon::Equip() 
{
	WeaponBox->SetSimulatePhysics(false);
	WeaponBox->SetEnableGravity(false);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	const USkeletalMeshSocket *RightHandSocket = GetAurora()->GetMesh()->GetSocketByName(FName("RightHandSocket"));

	if(RightHandSocket)
	{
		RightHandSocket->AttachActor(this, GetAurora()->GetMesh());
		Aurora->SetEquippedWeapon(this);
		bShouldRotateWeapon = false;
	}
	WeaponWidget->SetVisibility(false);
}

void AWeapon::Drop() 
{
	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
	WeaponBox->DetachFromComponent(DetachmentRules);
	WeaponBox->SetSimulatePhysics(true);
	WeaponBox->SetEnableGravity(true);
	WeaponBox->SetLinearDamping(LinearDamping);
	WeaponBox->SetAngularDamping(AngularDamping);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	Aurora->SetEquippedWeapon(nullptr);

	GetWorldTimerManager().SetTimer(WeaponTimer, this, &AWeapon::WeaponAfterDrop, 2.f);
}

void AWeapon::ActivateBladeCollision() 
{
	WeaponBladeBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DeactivateBladeCollision() 
{
	WeaponBladeBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::WeaponAfterDrop() 
{
	FRotator Rotation = FRotator(0.f, 0.f, 0.f);
	WeaponBox->SetWorldRotation(Rotation);
	bShouldRotateWeapon = true;
}



