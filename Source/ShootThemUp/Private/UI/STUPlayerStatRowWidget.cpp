// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUPlayerStatRowWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void USTUPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
    if(PlayerNameTextBlock)
    {
        PlayerNameTextBlock->SetText(Text);
    }
}

void USTUPlayerStatRowWidget::SetKills(const FText& Text)
{
    if(KillsTextBlock)
    {
        KillsTextBlock->SetText(Text);
    }
}

void USTUPlayerStatRowWidget::SetDeaths(const FText& Text)
{
    if(DeathsTextBlock)
    {
        DeathsTextBlock->SetText(Text);
    }
}

void USTUPlayerStatRowWidget::SetTeam(const FText& Text)
{
    if(TeamTextBlock)
    {
        TeamTextBlock->SetText(Text);
    }
}

void USTUPlayerStatRowWidget::SetPlayerIndicatorVisibility(bool bVisible)
{
    if(PlayerIndicatorImage)
    {
        PlayerIndicatorImage->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void USTUPlayerStatRowWidget::SetTeamColor(const FLinearColor& Color)
{
    if(!TeamImage)
    {
        return;
    }

    TeamImage->SetColorAndOpacity(Color);
}
