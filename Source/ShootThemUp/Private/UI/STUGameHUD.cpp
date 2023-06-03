// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUGameHUD.h"

#include "STUGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD()
{

    Super::DrawHUD();
    //DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    check(GetWorld());
    
    GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(ESTUMatchState::Pause, CreateWidget(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget(GetWorld(), GameOverWidgetClass));

    for(const TTuple<ESTUMatchState, UUserWidget*> GameWidgetPair: GameWidgets)
    {
        UUserWidget* GameWidget = GameWidgetPair.Value;
        if(!GameWidget)
        {
            continue;
        }

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if(ASTUGameModeBase* GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->OnMatchStateChange.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
    }
}

void ASTUGameHUD::DrawCrossHair()
{
    if(!Canvas)
    {
        return;
    }
   
    const TInterval<float> Center(Canvas->SizeX*0.5, Canvas->SizeY*0.5);
    constexpr float HalfLineSize = 10.0f;
    constexpr float LineThickness = 2.0f;
    const FLinearColor LinearColor = FLinearColor::Green;
    
    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LinearColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LinearColor, LineThickness);
}

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
    if(CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if(GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }

    if(CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }
    
    UE_LOG(LogSTUGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}
