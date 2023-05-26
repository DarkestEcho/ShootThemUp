// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STURespawnComponent.h"

#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
    RespawnCountDown = RespawnTime;

    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.0f, true);
}

int32 USTURespawnComponent::GetRespawnCountDown() const
{
    return RespawnCountDown;
}

bool USTURespawnComponent::IsRespawnInProgress() const
{
    return GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}

void USTURespawnComponent::RespawnTimerUpdate()
{
    if(--RespawnCountDown == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

        if(ASTUGameModeBase* GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
        {
            GameMode->RespawnRequest(Cast<AController>(GetOwner()));
        }
    }
}
