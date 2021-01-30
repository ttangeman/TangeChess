#pragma once

#include "Core/Common.h"
#include "Core/IEvent.h"

namespace Tange
{
    // See: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    enum class InputEvent
    {
        // Mouse
        LeftClick,
        RightClick,
        MiddleClick,

        KeyBackspace = 0x08,
        KeyTab,
        KeyEnter = 0x0D,
        KeyShift = 0x10,
        KeyControl,
        KeyAlt,
        KeyEscape = 0x1B,
        KeySpace = 0x20,

        KeyLeftArrow = 0x25,
        KeyUpArrow,
        KeyRightArrow,
        KeyDownArrow,

        Key0 = 0x30,
        Key2,
        Key3,
        Key4,
        Key5,
        Key6,
        Key7,
        Key8,
        Key9,

        KeyA = 0x41,
        KeyB,
        KeyC,
        KeyD,
        KeyE,
        KeyF,
        KeyG,
        KeyH,
        KeyI,
        KeyJ,
        KeyK,
        KeyL,
        KeyM,
        KeyN,
        KeyO,
        KeyP,
        KeyQ,
        KeyR,
        KeyS,
        KeyT,
        KeyU,
        KeyV,
        KeyW,
        KeyX,
        KeyY,
        KeyZ,

        KeyNumpad0 = 0x60,
        KeyNumpad1,
        KeyNumpad2,
        KeyNumpad3,
        KeyNumpad4,
        KeyNumpad5,
        KeyNumpad6,
        KeyNumpad7,
        KeyNumpad8,
        KeyNumpad9,

        KeyF1 = 0x70,
        KeyF2,
        KeyF3,
        KeyF4,
        KeyF5,
        KeyF6,
        KeyF7,
        KeyF8,
        KeyF9,
        KeyF10,
        KeyF11,
        KeyF12,
    };

    struct KeyPressed : public IEvent
    {
        EVENT_BOILERPLATE()

        InputEvent Key;

        KeyPressed(InputEvent event)
            : Key(event)
        {
        }
    };

    struct KeyReleased : public IEvent
    {
        EVENT_BOILERPLATE()

        InputEvent Key;

        KeyReleased(InputEvent event)
            : Key(event)
        {
        }
    };

    struct MouseClicked : public IEvent
    {
        EVENT_BOILERPLATE()

        InputEvent Button;
        Vec2i MousePosition;

        MouseClicked(InputEvent event, Vec2i position)
            : Button(event), MousePosition(position)
        {
        }
    };

    struct MouseReleased : public IEvent
    {
        EVENT_BOILERPLATE()

        InputEvent Button;
        Vec2i MousePosition;

        MouseReleased(InputEvent event, Vec2i position)
            : Button(event), MousePosition(position)
        {
        }
    };

    struct MouseMoved : public IEvent
    {
        EVENT_BOILERPLATE()

        Vec2i MousePosition;

        MouseMoved(Vec2i position)
            : MousePosition(position)
        {
        }
    };
}