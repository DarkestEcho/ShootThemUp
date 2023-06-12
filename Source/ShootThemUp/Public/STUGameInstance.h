// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Engine/GameInstance.h"
#include "STUGameInstance.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    FLevelData GetCurrentLevel() const;
    void SetCurrentLevel(const FLevelData& Level);
    
    FName GetLevelName(ESTULevels Level) const;

    TArray<FLevelData> GetLevelsData() const;

  
protected:
    UPROPERTY(EditDefaultsOnly, Category="Game")
    TArray<FLevelData> LevelsData;
    
    UPROPERTY(EditDefaultsOnly, Category="Game")
    TMap<ESTULevels, FName> LevelsName;

private:
    FLevelData CurrentLevel;
};
