// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUGameOverWidget.h"

#include "STUGameModeBase.h"
#include "STUPlayerState.h"
#include "STUPlayerStatRowWidget.h"
#include "STUUtils.h"
#include "Components/VerticalBox.h"

bool USTUGameOverWidget::Initialize()
{
    if(ASTUGameModeBase* GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->OnMatchStateChange.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
    }
    
    return Super::Initialize();
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
    if(State == ESTUMatchState::GameOver)
    {
        UpdatePlayerStat();
    }
}

void USTUGameOverWidget::UpdatePlayerStat()
{
    if(!PlayerStatBox)
    {
        return;
    }

    PlayerStatBox->ClearChildren();

    for(FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        AController* Controller = It->Get();
        if(!Controller)
        {
            continue;
        }

        ASTUPlayerState* PlayerState= Controller->GetPlayerState<ASTUPlayerState>();
        if(!PlayerState)
        {
            continue;
        }

        USTUPlayerStatRowWidget* PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if(!PlayerStatRowWidget)
        {
            continue;
        }

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetKills(STUUtils::GetTextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRowWidget->SetDeaths(STUUtils::GetTextFromInt(PlayerState->GetDeathsNum()));
        PlayerStatRowWidget->SetTeam(STUUtils::GetTextFromInt(PlayerState->GetTeamID()));
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());

        PlayerStatBox->AddChild(PlayerStatRowWidget);
    }
}
