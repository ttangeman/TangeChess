#include "Platform/Input.h"

namespace Platform
{
    InputHandler& InputHandler::GetInstance()
    {
        // NOTE: This is allegedly thread-safe in C++11!
        static InputHandler instance;
        return instance;
    }

    void InputHandler::Initialize(WindowHandle window)
    {
        m_window = window;
    }

    bool InputHandler::IsCurrentlyPressed(InputEvent event) const 
    {
        const InputState& eventState = GetEvent(event);
        return eventState.IsActive;
    }

    bool InputHandler::WasPressed(InputEvent event)
    {
        InputState& eventState = GetEvent(event);

        if (eventState.WasActive)
        {
            eventState.WasActive = false;
            return true;
        }

        return false;
    }

    void InputHandler::UpdateInputState(InputEvent event, bool isDown, bool isUp)
    {
        auto& eventState = m_inputState.at((usize)event);
        
        if (isDown) 
        {
            if (!eventState.IsActive)
            {
                eventState.IsActive = true;
                eventState.WasActive = true;        
            }
        }
        
        if (isUp) 
        {
            eventState.IsActive = false;
            eventState.WasActive = false;
        }
    }

    void InputHandler::UpdateMousePosition()
    {
        POINT cursorPosition;
        // Gets the mouse position relative to the screen.
        GetCursorPos(&cursorPosition);
        // This maps the mouse position into the window's dimensions.
        ScreenToClient(m_window, &cursorPosition);
        
        RECT clientRect;
        GetClientRect(m_window, &clientRect);
        auto clientHeight = clientRect.bottom - clientRect.top;

        // Flip the Y to match D3D11's pixel coordinate space.
        // (Bottom-up instead of top-down).
        // TODO: Couldn't this just be a - instead of an abs?
        cursorPosition.y = AbsoluteValue(cursorPosition.y - clientHeight);
        MousePosition = Vec2i(cursorPosition.x, cursorPosition.y);
    }

    const InputState& InputHandler::GetEvent(InputEvent event) const
    {
        return m_inputState.at((usize)event);
    }

    InputState& InputHandler::GetEvent(InputEvent event) 
    {
        return m_inputState.at((usize)event);
    }
}