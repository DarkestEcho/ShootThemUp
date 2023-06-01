// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUPauseWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

bool USTUPauseWidget::Initialize()
{
    const bool InitStatus = Super::Initialize();

    if(ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
    }

    return InitStatus;
}

void USTUPauseWidget::OnClearPause()
{
    if(GetWorld()->GetAuthGameMode())
    {
        GetWorld()->GetAuthGameMode()->ClearPause();
    }
}
