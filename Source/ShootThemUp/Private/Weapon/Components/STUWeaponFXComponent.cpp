// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

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
    UNiagaraSystem* Effect = DefaultEffect;

    if (Hit.PhysMaterial.IsValid())
    {
        if(const UPhysicalMaterial* PhysMaterial = Hit.PhysMaterial.Get(); EffectsMap.Contains(PhysMaterial))
        {
            Effect = EffectsMap[PhysMaterial];
        }
    }
    
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
}
