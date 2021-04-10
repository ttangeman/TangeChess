#pragma once

#include "Core/Common.h"

namespace Tange
{
    static int32 _EventAccumulator = 0;

    class IEvent
    {
    public:
        virtual ~IEvent()
        {
        }
    };

    template<typename Derived>
    class Event : public IEvent
    {
    protected:
        friend class EventManager;

        static int32 GetIndex()
        {
            // Lazily initialize the event index.
            static int32 m_eventIndex = _EventAccumulator++;
            return m_eventIndex;
        }
    };
}