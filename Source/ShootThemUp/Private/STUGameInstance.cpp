// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameInstance.h"

#include "STUSoundFuncLib.h"


FName USTUGameInstance::GetLevelName(ESTULevels Level) const
{
    return LevelsName.Contains(Level) ? LevelsName[Level] : NAME_None;
}

TArray<FLevelData> USTUGameInstance::GetLevelsData() const
{
    return LevelsData;
}

void USTUGameInstance::ToggleVolume()
{
    USTUSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}

FLevelData USTUGameInstance::GetCurrentLevel() const
{
    return CurrentLevel;
}

void USTUGameInstance::SetCurrentLevel(const FLevelData& Level)
{
    CurrentLevel = Level;
}
