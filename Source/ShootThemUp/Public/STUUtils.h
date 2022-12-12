#pragma once


class STUUtils
{
public:
    template<class T>
    static T* GetComponentFromPawn(const APawn* Pawn)
    {
        if(!Pawn)
        {
            return nullptr;
        }
        return Cast<T>(Pawn->GetComponentByClass(T::StaticClass()));
    }
};