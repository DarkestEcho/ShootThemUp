// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STULevelItemWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTULevelItemWidget, All, All);

void USTULevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    if(LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
    }
}

void USTULevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}


void USTULevelItemWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if(LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
    }

    if(LevelImage)
    {
        LevelImage->SetBrushFromTexture(Data.LevelThumb);
    }
}

FLevelData USTULevelItemWidget::GetLevelData() const
{
    return LevelData;
}

void USTULevelItemWidget::SetSelected(bool bSelected)
{
    if(FrameImage)
    {
        FrameImage->SetVisibility(bSelected? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}
