#pragma once

#include "Platform/PlatformWrapper.h"
#include "Core/EventManager.h"
#include "Platform/InputEvents.h"
#include "Platform/WindowEvents.h"

// NOTE: These classes should be implemented for the platforms
// that are supported by the application. For example, Windows
// should have a Win32Manager file with the implementations of
// Stopwatch and PlatformManager.
// NOTE: PlatformManager is not an abstract class because there should only
// ever be one type of PlatformManager at a time.

namespace Tange
{
    class Stopwatch 
    {
        // Used for calculating the time, if needed.
        int64 m_clocksPerSecond;

        // Keeps track of the time/clock count.
        int64 m_currentCount;

    public:
        void Start();
        float Stop() const;
    
        Stopwatch();

    private:
        // Queries the OS for the CPU clocks per second if it is needed
        // for accurate timing, like on Windows.
        int64 QueryClock() const;
    };

    class PlatformManager 
    {
        static PlatformManager s_instance;

        // Platform-specific window handle.
        WindowHandle m_hWindow;
        std::atomic<bool> m_shouldQuit;

        // Tracks the render dimensions of the window.
        // It is stored because the old dimensions want to be known
        // when the window is resized (WM_SIZE auto resizes the 
        // window rect and only provides the new dimensions).
        Vec2 m_renderDimensions;

        // Tracks the window dimensions (including borders, etc.).
        Vec2 m_windowDimensions;

    public:
        PlatformManager(const PlatformManager&) = delete;
        void operator=(const PlatformManager&) = delete;

        // Finds the mouse position.
        static Vec2 CalculateMousePosition();
        
        // Dispatches messages from the system to the event manager,
        // such as keyboard and mouse input.
        static void DispatchSystemMessages();

        // Initializes the OS and creates a window.
        static bool InitializeAndCreateWindow(const char* windowTitle, int32 windowWidth, int32 windowHeight, 
                                              bool showCursor, bool windowBorders);
        static void Shutdown();
        
        static bool ShouldQuit();
        static void ForceQuit();

        static Vec2 GetRenderDimensions();
        // Finds the dimensions of the actual window client minus the border.
        static Vec2 RecomputeRenderDimensions();

        static Vec2 GetWindowDimensions();
        // Finds the dimensions of the actual window client minus the border.
        static Vec2 RecomputeWindowDimensions();

        static WindowHandle GetWindow();
        
    private:
        PlatformManager() = default;
    };
}