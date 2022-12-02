// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUGameHUD.h"

#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD()
{

    Super::DrawHUD();
    DrawCrossHair();
}

void ASTUGameHUD::DrawCrossHair()
{
    if(!Canvas)
    {
        return;
    }
   
    const TInterval<float> Center(Canvas->SizeX*0.5, Canvas->SizeY*0.5);
    constexpr float HalfLineSize = 10.0f;
    constexpr float LineThickness = 2.0f;
    const FLinearColor LinearColor = FLinearColor::Green;
    
    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LinearColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LinearColor, LineThickness);
}
