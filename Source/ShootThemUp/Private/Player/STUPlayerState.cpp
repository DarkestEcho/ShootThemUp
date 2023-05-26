// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All);

void ASTUPlayerState::SetTeamID(int32 ID)
{
    TeamID = ID;
}

int32 ASTUPlayerState::GetTeamID() const
{
    return TeamID;
}

void ASTUPlayerState::SetTeamColor(const FLinearColor& Color)
{
    TeamColor = Color;
}

FLinearColor ASTUPlayerState::GetTeamColor() const
{
    return TeamColor;
}

void ASTUPlayerState::AddKill()
{
    ++KillsNum;
}

int32 ASTUPlayerState::GetKillsNum() const
{
    return KillsNum;
}

void ASTUPlayerState::AddDeath()
{
    ++DeathsNum;
}

int32 ASTUPlayerState::GetDeathsNum() const
{
    return DeathsNum;
}

void ASTUPlayerState::LogInfo() const
{
    UE_LOG(LogSTUPlayerState, Display, TEXT("Team ID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}
