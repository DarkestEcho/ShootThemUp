// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/STUMenuWidget.h"

#include "STUGameInstance.h"
#include "STULevelItemWidget.h"
#include "STUOpenLevelWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(GetWorld());

    if(QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
    }

    InitLevelItems();
}

void USTUMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USTUMenuWidget::InitLevelItems()
{
    const USTUGameInstance* STUGameInstance = GetSTUGameInstance();
    if(!STUGameInstance)
    {
        return;
    }

    checkf(STUGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty!"));

    if(!LevelItemsBox)
    {
        return;
    }

    LevelItemsBox->ClearChildren();

    for(FLevelData LevelData: STUGameInstance->GetLevelsData())
    {
        USTULevelItemWidget* LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if(!LevelItemWidget)
        {
            continue;
        }
        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);
        UE_LOG(LogSTUMenuWidget, Display, TEXT("OnLevelSelected is %s"), *FString(LevelItemWidget->OnLevelSelected.IsBound()? "Bound" : "Not Bound"));

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }
  
    if(STUGameInstance->GetCurrentLevel().LevelDisplayName.IsNone())
    {
        OnLevelSelected(STUGameInstance->GetLevelsData()[0]);
    }
    else
    {
        OnLevelSelected(STUGameInstance->GetCurrentLevel());
    }
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
    UE_LOG(LogSTUMenuWidget, Display, TEXT("OnLevelSelected"));

    USTUGameInstance* STUGameInstance = GetSTUGameInstance();
    if(!STUGameInstance)
    {
        return;
    }

    STUGameInstance->SetCurrentLevel(Data);
    StartGameButton->SetLevel(Data.Level);

    for(USTULevelItemWidget* LevelItemWidget : LevelItemWidgets)
    {
        if(LevelItemWidget)
        {
            const bool bSelected = Data.Level == LevelItemWidget->GetLevelData().Level;
            LevelItemWidget->SetSelected(bSelected);
            UE_LOG(LogSTUMenuWidget, Display, TEXT("Widget %s selected: %s"), *LevelItemWidget->GetName(), *FString(bSelected ? "True" : "False"));
        }
    }
}

USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const
{
    return GetWorld()->GetGameInstance<USTUGameInstance>();
}
