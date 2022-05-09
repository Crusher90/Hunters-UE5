// Fill out your copyright notice in the Description page of Project Settings.


#include "AuroraPlayerController.h"
#include "Blueprint/UserWidget.h"

AAuroraPlayerController::AAuroraPlayerController()
{

}

void AAuroraPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    if(HudWidget)
    {
        UserWidget = CreateWidget<UUserWidget>(this, HudWidget);
    }
    UserWidget->AddToViewport();
    UserWidget->SetVisibility(ESlateVisibility::Visible);

    // if (WPauseMenu)
	// {
	// 	PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
	// 	if (PauseMenu)
	// 	{
	// 		PauseMenu->AddToViewport();
	// 		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
	// 	}
	// }
}

void AAuroraPlayerController::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

}

// void AAuroraPlayerController::DisplayPauseMenu_Implementation() 
// {
//     bPauseMenuOpen = true;
// 	bShowMouseCursor = true;

// 	FInputModeGameAndUI InputModeGameAndUI;
// 	SetInputMode(InputModeGameAndUI);
// }

// void AAuroraPlayerController::RemovePauseMenu_Implementation() 
// {
//     bPauseMenuOpen = false;
// 	bShowMouseCursor = false;
// }

// void AAuroraPlayerController::TogglePauseMenu() 
// {
// 	if (!bPauseMenuOpen)
// 	{
// 		DisplayPauseMenu();
// 	}
// 	else
// 	{
// 		RemovePauseMenu();
// 	}
// }



