// Fill out your copyright notice in the Description page of Project Settings.


#include "Aurora.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Explosion.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AuroraPlayerController.h"
#include "Enemy.h"

// Sets default values
AAurora::AAurora()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 100.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// default values
	LookUpRate = 45.f;
	LookRightRate = 45.f;
	bHasWeapon = false;
	AuroraMaxHealth = 100.f;
	AuroraHealth = 100.f;
	AuroraMaxStamina = 100.f;
	AuroraStamina = 100.f;
	Coins = 0;
	bAttacking = false;
	bSprint = false;
	// bESCDown = false;
}

// Called when the game starts or when spawned
void AAurora::BeginPlay()
{
	Super::BeginPlay();
	AuroraController = Cast<AAuroraPlayerController>(GetController());

	GetMesh()->HideBoneByName(FName("weapon"), EPhysBodyOp::PBO_None);
}

void AAurora::MoveForward(float Value) 
{
	if((Controller != nullptr) && (Value != 0) && (!bAttacking))
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
		FVector Direction{FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X)};

		AddMovementInput(Direction, Value);
	}
}

void AAurora::MoveRight(float Value) 
{
	if((Controller != nullptr) && (Value != 0) && (!bAttacking))
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
		FVector Direction{FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y)};

		AddMovementInput(Direction, Value);
	}
}

void AAurora::LookUp(float Value) 
{
	AddControllerPitchInput(Value * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAurora::LookRight(float Value) 
{
	AddControllerYawInput(Value * LookRightRate * GetWorld()->GetDeltaSeconds());
}

void AAurora::Attack() 
{
	if(!bAttacking)
	{
		bAttacking = true;
		class UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance)
		{
			int32 RandomSection = FMath::RandRange(0, 3);
			switch (RandomSection)
			{
			case 0:
				AnimInstance->Montage_Play(AuroraCombatMontage);
				AnimInstance->Montage_JumpToSection(FName("Attack_A"), AuroraCombatMontage);
				break;
			case 1:
				AnimInstance->Montage_Play(AuroraCombatMontage);
			    AnimInstance->Montage_JumpToSection(FName("Attack_B"), AuroraCombatMontage);
				break;
			case 2:
				AnimInstance->Montage_Play(AuroraCombatMontage);
				AnimInstance->Montage_JumpToSection(FName("Attack_C"), AuroraCombatMontage);
				break;
			case 3:
				AnimInstance->Montage_Play(AuroraCombatMontage);
				AnimInstance->Montage_JumpToSection(FName("Attack_D"), AuroraCombatMontage);
				break;
			}
		}
	}
}

void AAurora::EKeyPressed() 
{
	if(bHasWeapon)
	{
		Attack();
	}
}

void AAurora::ShiftKeyPressed() 
{
	bSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AAurora::ShiftKeyReleased() 
{
	bSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AAurora::AttackEnd() 
{
	bAttacking = false;
}


void AAurora::FKeyPressed() 
{
	if((EquippedWeapon) && (!bHasWeapon))
	{
		EquippedWeapon->Equip();
		bHasWeapon = true;
		if(WeaponEquipSound)
		{
			UGameplayStatics::PlaySound2D(this, WeaponEquipSound);
		}
	}
}

void AAurora::GKeyPressed() 
{
	if(bHasWeapon)
	{
		EquippedWeapon->Drop();
		bHasWeapon = false;
	}
}

float AAurora::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) 
{
	if(AuroraHealth - DamageAmount <= 0.f)
	{
		AuroraHealth = 0;
		Die();
	}
	else
	{
		AuroraHealth -= DamageAmount;
	}
	return DamageAmount;
}

void AAurora::IncrementCoin() 
{
	Coins += 1;
}

void AAurora::IncrementHealth() 
{
	if(AuroraHealth <= 50)
	{
		AuroraHealth += 50.f;
	}
	else
	{
		AuroraHealth = AuroraMaxHealth;
	}
}

void AAurora::ActivateBladeCollision() 
{
	if(EquippedWeapon)
	{
		EquippedWeapon->ActivateBladeCollision();
	}
}

void AAurora::DeactivateBladeCollision() 
{
	if(EquippedWeapon)
	{
		EquippedWeapon->DeactivateBladeCollision();
	}
}

void AAurora::DeathEnd() 
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AAurora::Die() 
{
	class UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->Montage_Play(AuroraCombatMontage);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = true;

	GetWorldTimerManager().SetTimer(OpenLevelTimer, this, &AAurora::OpenLevelWhenDie, 2.f);

	// GetWorldTimerManager().SetTimer(DestroyTimer, this, &AAurora::DestroyAurora, 3.f);
}

void AAurora::SwitchLevel(FName LevelName) 
{
	FString CurrentLevel = GetWorld()->GetMapName();
	FName CurrentLevelName(*CurrentLevel);
	if(CurrentLevelName != LevelName)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	}
}

void AAurora::OpenLevelWhenDie() 
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("ElvenRuins"));
}

// void AAurora::DestroyAurora() 
// {
// 	Destroy();
// }

// void AAurora::ESCDown() 
// {
// 	bESCDown = true;

// 	if (AuroraController)
// 	{
// 		AuroraController->TogglePauseMenu();
// 	}
// }

// void AAurora::ESCUp() 
// {
// 	bESCDown = false;
// }

// Called every frame
void AAurora::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bSprint && AuroraStamina <= 0)
	{
		AuroraStamina = 0;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
	if(bSprint && AuroraStamina > 0)
	{
		AuroraStamina -= 5 * DeltaTime;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	if(!bSprint && AuroraStamina <= 0)
	{
		AuroraStamina += 5 * DeltaTime;
	}
	if(!bSprint && AuroraStamina <= 100)
	{
		AuroraStamina += 5 * DeltaTime;
	}
	if(AuroraStamina > AuroraMaxStamina)
	{
		AuroraStamina = AuroraMaxStamina;
	}
}

// Called to bind functionality to input
void AAurora::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AAurora::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AAurora::MoveRight);
	PlayerInputComponent->BindAxis(FName("LookUpDown"), this, &AAurora::LookUp);
	PlayerInputComponent->BindAxis(FName("LookRightLeft"), this, &AAurora::LookRight);

	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(FName("Equip"), EInputEvent::IE_Pressed, this, &AAurora::FKeyPressed);
	PlayerInputComponent->BindAction(FName("Drop"), EInputEvent::IE_Pressed, this, &AAurora::GKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), EInputEvent::IE_Pressed, this, &AAurora::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Sprint"), EInputEvent::IE_Pressed, this, &AAurora::ShiftKeyPressed);
	PlayerInputComponent->BindAction(FName("Sprint"), EInputEvent::IE_Released, this, &AAurora::ShiftKeyReleased);
	// PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AAurora::ESCDown);
	// PlayerInputComponent->BindAction("ESC", IE_Released, this, &AAurora::ESCUp);
}

