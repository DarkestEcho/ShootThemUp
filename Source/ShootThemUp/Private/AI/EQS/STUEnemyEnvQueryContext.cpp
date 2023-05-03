 // Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/STUEnemyEnvQueryContext.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

 void USTUEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
 {
     AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
     
     const UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);

     if(!Blackboard)
     {
         return;
     }

     const AActor* ContextActor = Cast<AActor>(Blackboard->GetValueAsObject(EnemyActorKeyName));
     
     UEnvQueryItemType_Actor::SetContextHelper(ContextData, ContextActor);
 }
