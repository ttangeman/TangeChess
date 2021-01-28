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

        static InputHandler& Get();
        InputHandler(const InputHandler&) = delete;
        void operator=(const InputHandler&) = delete;
        
        void Initialize(WindowHandle window);

        bool IsCurrentlyPressed(InputEvent event) const;
        bool WasPressed(InputEvent event);
        
        void UpdateInputState(InputEvent event, bool isDown, bool isUp);
        void UpdateMousePosition();

        private:

        static const int32 m_inputEventCount = (int32)InputEvent::Count;
        std::array<InputState, m_inputEventCount> m_inputState;
        
        WindowHandle m_hWindow;

        InputHandler() = default;

        const InputState& GetEvent(InputEvent event) const;
        InputState& GetEvent(InputEvent event);
    };
}
