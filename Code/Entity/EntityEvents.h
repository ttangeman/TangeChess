#pragma once

#include "Core/Common.h"
#include "Core/IEvent.h"

namespace Tange
{
    struct UpdateEvent : IEvent
    {
        EVENT_BOILERPLATE()

    };

    struct RenderEvent : IEvent
    {
        EVENT_BOILERPLATE()
    };
}