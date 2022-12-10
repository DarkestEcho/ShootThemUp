#pragma once

class AnimUtils
{
public:
    template<class T>
    static T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if (!Animation)
        {
            return nullptr;
        }

        const TArray<FAnimNotifyEvent> NotifyEvents = Animation->Notifies;

        for (const FAnimNotifyEvent& NotifyEvent : NotifyEvents)
        {
            if (T* AnimNotify = Cast<T>(NotifyEvent.Notify))
            {
                return AnimNotify;
            }
        }

        return nullptr;
    }
};