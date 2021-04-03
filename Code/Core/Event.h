#pragma once

#include "Core/Common.h"

static int32 _EventAccumulator = 0;

namespace Tange
{
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
            // Lazyily initialize the event index.
            static int32 m_eventIndex = _EventAccumulator++;
            return m_eventIndex;
        }

    public:
        virtual ~Event()
        {
        }
    };
}