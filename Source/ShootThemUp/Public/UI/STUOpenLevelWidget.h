// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "STUOpenLevelWidget.generated.h"

class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUOpenLevelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void SetLevel(ESTULevels NewLevel);
    
protected:
    UPROPERTY(meta=(BindWidget))
    UButton* OpenLevelButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Level")
    ESTULevels Level = ESTULevels::MainMenu;
    
    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnOpenLevel();
};
