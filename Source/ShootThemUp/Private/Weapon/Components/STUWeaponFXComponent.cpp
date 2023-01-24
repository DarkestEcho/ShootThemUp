// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

// Sets default values for this component's properties
USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
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

    if(DecalComponent){}
}
