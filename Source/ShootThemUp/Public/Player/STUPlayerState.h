// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
    void SetTeamID(int32 ID);
    int32 GetTeamID() const;

    void SetTeamColor(const FLinearColor& Color);
    FLinearColor GetTeamColor() const;

    void AddKill();
    int32 GetKillsNum() const;
    
    void AddDeath();
    int32 GetDeathsNum() const;
    
    void LogInfo() const;
    
private:
    int32 TeamID;
    FLinearColor TeamColor;

    int32 KillsNum = 0;
    int32 DeathsNum = 0;
};
