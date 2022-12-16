#pragma once


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
};