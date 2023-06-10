// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/STUMenuHUD.h"

#include "Blueprint/UserWidget.h"

void ASTUMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if(UUserWidget* MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass))
    {
        MenuWidget->AddToViewport();
    }
}
