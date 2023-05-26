// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STUPlayerController.h"

#include "STURespawnComponent.h"

ASTUPlayerController::ASTUPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}
