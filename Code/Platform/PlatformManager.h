#pragma once

#include "Platform/PlatformWrapper.h"
#include "Core/EventManager.h"
#include "Platform/InputEvents.h"

// NOTE: These classes should be implemented for the platforms
// that are supported by the application. For example, Windows
// should have a Win32Manager file with the implementations of
// Stopwatch and PlatformManager.
// NOTE: PlatformManager is not an abstract class because there should only
// ever be one type of PlatformManager at a time.

namespace Platform
{
    class Stopwatch 
    {
        public:
        
        void Start();
        float Stop() const;
        
        Stopwatch();

        private:
        
        // Used for calculating the time, if needed.
        int64 m_clocksPerSecond;
        
        // Keeps track of the time/clock count.
        int64 m_currentCount; 
        
        // Queries the OS for the CPU clocks per second if it is needed
        // for accurate timing, like on Windows.
        int64 QueryClock() const;
    };

    class PlatformManager 
    {
        public:
        
        static PlatformManager& Get();
        PlatformManager(const PlatformManager&) = delete;
        void operator=(const PlatformManager&) = delete;
        
        // Dispatches messages from the system to the event manager,
        // such as keyboard and mouse input.
        void DispatchSystemMessages();

        // Initializes the OS and creates a window.
        bool InitializeAndCreateWindow(const char* windowTitle, int32 windowWidth, int32 windowHeight, 
                                       bool showCursor, bool windowBorders);
        void Shutdown();
        
        bool ShouldQuit() const;

        static void ForceQuit();

        // Finds the dimensions of the actual window client minus the border.
        Vec2 GetRenderDimensions() const;
        
        WindowHandle GetWindow() const;
        
        private:

        // Platform-specific window handle.
        WindowHandle m_hWindow;
        std::atomic<bool> m_shouldQuit;

        PlatformManager() = default;
    };
}