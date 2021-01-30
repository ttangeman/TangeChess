#pragma once

#include "Core/Common.h"

static int32 _EventAccumulator = 0;

#define EVENT_BOILERPLATE() private: \
        static inline int32 m_eventIndex = -1; \
    public: \
        static int32 GetIndex() \
        { \
            return m_eventIndex; \
        } \
        static void Initialize() \
        { \
            if (!IsInitialized()) \
            { \
                m_eventIndex = _EventAccumulator++; \
            } \
        } \
        static bool IsInitialized() \
        { \
            return m_eventIndex != -1; \
        }

namespace Tange
{
    // NOTE: Each event needs the EVENT_BOILERPLATE macro
    struct IEvent
    {
    };
}