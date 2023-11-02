// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "STUMenuWidget.generated.h"

class USTUOpenLevelWidget;
class UButton;
class UHorizontalBox;
class USTUGameInstance;
class USTULevelItemWidget;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta=(BindWidget))
    UButton* QuitGameButton;

    UPROPERTY(meta=(BindWidget))
    USTUOpenLevelWidget* StartGameButton;

    UPROPERTY(meta=(BindWidget))
    UHorizontalBox* LevelItemsBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
    USoundCue* StartGameSound;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<USTULevelItemWidget*> LevelItemWidgets;

    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();

    void InitLevelItems();

    UFUNCTION()
    void OnLevelSelected(const FLevelData& Data);

    USTUGameInstance* GetSTUGameInstance() const;
};
