// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\UI\STUOpenLevelWidget.h"

#include "STUGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUOpenLevelWidget, All, All);

void USTUOpenLevelWidget::SetLevel(ESTULevels NewLevel)
{
    Level = NewLevel;
}

void USTUOpenLevelWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(OpenLevelButton)
    {
        OpenLevelButton->OnClicked.AddDynamic(this, &USTUOpenLevelWidget::OnOpenLevel);
        return;
    }
}

void USTUOpenLevelWidget::OnOpenLevel()
{
    const USTUGameInstance* GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if(!GameInstance)
    {
        return;
    }

    if(GameInstance->GetLevelName(Level).IsNone())
    {
        UE_LOG(LogSTUOpenLevelWidget, Error, TEXT("Level name is NONE"));
        return;
    }

    UE_LOG(LogSTUOpenLevelWidget, Display, TEXT("Open Level: %s"), *GameInstance->GetLevelName(Level).ToString());
    UGameplayStatics::OpenLevel(this, GameInstance->GetLevelName(Level));
}
