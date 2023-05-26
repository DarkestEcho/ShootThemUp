// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUGameDataWidget.h"

#include "STUGameModeBase.h"
#include "STUPlayerState.h"

int32 USTUGameDataWidget::GetKillsNum() const
{
    const ASTUPlayerState* PlayerState = GetSTUPlayerState();
    return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 USTUGameDataWidget::GetCurrentRoundNum() const
{
    const ASTUGameModeBase* GameMode = GetSTUGameMode();
    return GameMode? GameMode->GetCurrentRoundNum() : 0;
}

int32 USTUGameDataWidget::GetTotalRoundsNum() const
{
    const ASTUGameModeBase* GameMode = GetSTUGameMode();
    return GameMode? GameMode->GetGameData().RoundsNum : 0;
}

int32 USTUGameDataWidget::GetRoundSecondRemaining() const
{
    const ASTUGameModeBase* GameMode = GetSTUGameMode();
    return GameMode? GameMode->GetRoundSecondRemaining() : 0;
}

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const
{
    return Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
}

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
    return GetOwningPlayer() ? GetOwningPlayer()->GetPlayerState<ASTUPlayerState>() : nullptr;
}
