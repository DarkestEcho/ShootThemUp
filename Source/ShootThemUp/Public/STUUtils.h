#pragma once
#include "STUPlayerState.h"


class STUUtils
{
public:
    template<class T>
    static T* GetComponentFromActor(const AActor* Actor)
    {
        if(!Actor)
        {
            return nullptr;
        }
        return Cast<T>(Actor->GetComponentByClass(T::StaticClass()));
    }

    bool static AreEnemies(const AController* Controller1, const AController* Controller2)
    {
        if(!Controller1 || !Controller2 || Controller1 == Controller2)
        {
            return false;
        }

        const ASTUPlayerState* PlayerState1 = Controller1->GetPlayerState<ASTUPlayerState>();
        const ASTUPlayerState* PlayerState2 = Controller2->GetPlayerState<ASTUPlayerState>();
        
        return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
    }
};
