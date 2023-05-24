// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STUPlayerState.h"

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
