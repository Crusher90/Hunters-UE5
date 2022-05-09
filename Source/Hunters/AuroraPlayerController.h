// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuroraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HUNTERS_API AAuroraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuroraPlayerController();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerController", meta = (AllowPrivateAccess = "true"))
	class UUserWidget *UserWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerController", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HudWidget;

public:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	// TSubclassOf<UUserWidget> WPauseMenu;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	// UUserWidget* PauseMenu;

	// UFUNCTION(BlueprintNativeEvent)
	// void DisplayPauseMenu();

	// UFUNCTION(BlueprintNativeEvent)
	// void RemovePauseMenu();

	// UFUNCTION(BlueprintCallable)
	// void TogglePauseMenu();

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	// bool bPauseMenuOpen;

	// void GameModeOnly();

};
