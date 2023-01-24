// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

// Sets default values for this component's properties
USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit) const
{
    FImpactData ImpactData = DefaultImpactData;

    if (Hit.PhysMaterial.IsValid())
    {
        if (const UPhysicalMaterial* PhysMaterial = Hit.PhysMaterial.Get(); ImpactDataMap.Contains(PhysMaterial))
        {
            ImpactData = ImpactDataMap[PhysMaterial];
        }
    }

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), //
        ImpactData.NiagaraEffect,                              //
        Hit.ImpactPoint,                                       //
        Hit.ImpactNormal.Rotation());

    UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), //
        ImpactData.DecalData.Material,                                                   //
        ImpactData.DecalData.Size,                                                       //
        Hit.ImpactPoint,                                                                 //
        Hit.ImpactNormal.Rotation());

    if(DecalComponent)
    {
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
    }
}