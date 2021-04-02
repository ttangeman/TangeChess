#pragma once

#include "Core/Common.h"
#include "Core/EntryPoint.h"
#include "Core/EventManager.h"
#include "Platform/PlatformWrapper.h"
#include "Platform/WindowEvents.h"
#include "Platform/InputEvents.h"
// TODO: Better way to include this?
#include "Render/D3D11/D3D11Renderer.h"
#include "Render/Renderer.h"
#include "Render/ResourceManager.h"
#include "Render/RenderQueue.h"
#include "World/WorldTransform.h"

// Unity build (single translation unit) includes.
#include "Core/EventManager.cpp"
#include "Platform/Win32/Win32Manager.cpp"
#include "Platform/Win32/Win32FileSystem.cpp"
#include "Asset/Image.cpp"
#include "Asset/FontAtlas.cpp"
#include "Render/D3D11/D3D11Renderer.cpp"
#include "Render/D3D11/D3D11ResourceManager.cpp"
#include "Render/D3D11/D3D11RenderQueue.cpp"
#include "Entity/EntityManager.cpp"
#include "UserInterface/GuiMenu.cpp"
#include "UserInterface/GuiButton.cpp"
#include "UserInterface/TextLine.cpp"

namespace Tange
{ 
    class Application
    {        
        // Timer for finding the elapsed time between updates.
        Stopwatch m_timer;
        
        // The desired update rate for the application. Defaults to 60Hz.
        float m_desiredUpdateRate;

    protected:
        // Stores the last elapsed time for updating and rendering the application.
        float m_lastFrameTime;
        float m_dTime;

    public:
        Application() = delete;
        Application(const std::string& title, int32 width, int32 height);
        virtual ~Application();

        // Runs the application loop which calls update and render.
        void Run();
        
        virtual void Update() = 0;
        virtual void Render() = 0;      
    };
}