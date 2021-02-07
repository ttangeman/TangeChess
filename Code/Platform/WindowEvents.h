#pragma once

#include "Core/IEvent.h"

namespace Tange
{
    struct WindowResized : public IEvent
    {
        EVENT_BOILERPLATE()

        int32 DesiredWidth;
        int32 DesiredHeight;
        // Used for calculating new bounding boxes for clickables.
        int32 CurrentWidth;
        int32 CurrentHeight;

        WindowResized(int32 desiredWidth, int32 desiredHeight,
                      int32 currentWidth, int32 currentHeight)
            : DesiredWidth(desiredWidth), DesiredHeight(desiredHeight),
              CurrentWidth(currentWidth), CurrentHeight(currentHeight)
        {
        }
    };
}