// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameInstance.h"


FName USTUGameInstance::GetLevelName(ESTULevels Level) const
{
    return LevelsName.Contains(Level) ? LevelsName[Level] : NAME_None;
}
