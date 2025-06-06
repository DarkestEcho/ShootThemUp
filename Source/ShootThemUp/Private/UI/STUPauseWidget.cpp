// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUPauseWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void USTUPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    if(ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
    }
}

void USTUPauseWidget::OnClearPause()
{
    if(GetWorld()->GetAuthGameMode())
    {
        GetWorld()->GetAuthGameMode()->ClearPause();
    }
}
