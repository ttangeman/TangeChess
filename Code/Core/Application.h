#pragma once

#include "Common.h"
#include "Platform/PlatformWrapper.h"

#include "Platform/Win32/Win32Manager.cpp"
#include "Platform/Win32/Win32Input.cpp"

namespace Core
{
    using namespace Platform;
    
    class Application
    {
        public:
        
        // Stores the last elapsed time for updating and rendering the application.
        float LastTimeStep;

        Application(const std::string& title, int32 width, int32 height);
        ~Application();

        // Runs the application loop which calls update and render.
        void Run();
        
        virtual void Update() = 0;
        virtual void Render() = 0;

        private:
        
        // Timer for finding the elapsed time between updates.
        Stopwatch m_timer;
        
        // The desired update rate for the application. Defaults to 60Hz.
        float m_desiredUpdateRate;
        
        Application();
    };
}