#pragma once

#include "Platform/PlatformWrapper.h"

namespace Platform
{
    enum class InputEvent 
    {
        // Mouse
        LeftClick,
        RightClick,
        MiddleClick,
        
        // Keyboard
        MoveForward, // Default: W
        MoveLeft, // Default: A
        MoveBackward, // Default: S
        MoveRight, // Default: D
        ToggleFullscreen, // Default: F
        SystemQuit, // Default: Q
        ToggleMainMenu, // Default: Escape

        // Always needs to be last
        Count
    };
    
    // HACK: So I can use InputEvent::Count to initialize a static array :)
    static const usize InputEventCount = (const usize)InputEvent::Count;

    struct InputState 
    {
        bool IsActive;
        bool WasActive;
    };

    // TODO: Not thread-safe currently!
    class InputHandler 
    {
        public:

        Vec2i MousePosition;

        static InputHandler& GetInstance();
        
        void Initialize(WindowHandle window);

        bool IsCurrentlyPressed(InputEvent event) const;
        bool WasPressed(InputEvent event);
        
        void UpdateInputState(InputEvent event, bool isDown, bool isUp);
        void UpdateMousePosition();

        private:

        std::array<InputState, InputEventCount> m_inputState;

        WindowHandle m_window;

        const InputState& GetEvent(InputEvent event) const;
        InputState& GetEvent(InputEvent event);

        InputHandler() {}
    };
}
