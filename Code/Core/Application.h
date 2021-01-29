#pragma once

#include "Core/Common.h"
#include "Core/EntryPoint.h"
#include "Core/EventManager.h"
#include "Platform/PlatformWrapper.h"
// TODO: Better way to include this?
#include "Render/D3D11/D3D11Renderer.h"
#include "Render/Renderer.h"
#include "Render/Transformable.h"
#include "Render/Drawable.h"
#include "Render/ResourceManager.h"

// Unity build (single translation unit) includes.
#include "Core/EventManager.cpp"
#include "Platform/Win32/Win32Manager.cpp"
#include "Platform/Win32/Win32FileManager.cpp"
#include "Asset/Image.cpp"
#include "Asset/FontAtlas.cpp"
#include "Render/D3D11/D3D11Renderer.cpp"
#include "Render/D3D11/D3D11Transformable.cpp"
#include "Render/D3D11/D3D11Drawable.cpp"
#include "Render/D3D11/D3D11ResourceManager.cpp"
#include "Entity/EntityManager.cpp"

namespace Core
{
    using namespace Platform;
    
    class Application
    {
        public:
        
        // Stores the last elapsed time for updating and rendering the application.
        float LastFrameTime;

        Application() = delete;
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
        
    };
}