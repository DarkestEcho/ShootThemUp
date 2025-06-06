// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Components/ActorComponent.h"
#include "STUWeaponFXComponent.generated.h"

class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponFXComponent();

    void PlayImpactFX(const FHitResult& Hit) const;
	    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    FImpactData DefaultImpactData;
 
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;
};