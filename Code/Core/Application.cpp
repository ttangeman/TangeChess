#include "Application.h"

namespace Tange
{
    Application::Application(const std::string& title, int32 width, int32 height)
    {
        // NOTE: The platform manager initializes the event manager
        // and registers input events for us.
        if (PlatformManager::InitializeAndCreateWindow(title.c_str(), width, 
                                                       height, true, true))
        {
            m_timer = Stopwatch();
            // 60 hertz is default.
            m_desiredUpdateRate = 0.0167f;
            
            // Initialize the renderer.
            IntializeRendererPipeline();
            ResourceManager::Initialize();

            // Register common entity components.
            EntityManager::RegisterComponent<Drawable>();
            EntityManager::RegisterComponent<Transformable>();
            EntityManager::RegisterComponent<Dragable2D>();
            EntityManager::RegisterComponent<Clickable2D>();
            EntityManager::RegisterComponent<Outline2D>();
            EntityManager::RegisterComponent<Text>();
        }
    }

    Application::~Application()
    {
        ShutdownRendererPipeline();
        PlatformManager::Shutdown();
    }

    void Application::Run()
    {
        while (!PlatformManager::ShouldQuit())
        {
            m_timer.Start();

            PlatformManager::DispatchSystemMessages();
            Update();
            Render();

            LastFrameTime = m_timer.Stop();

            if (LastFrameTime < m_desiredUpdateRate)
            {
                // Need to convert from seconds to milliseconds.
                auto sleepTime = (uint64)((m_desiredUpdateRate - LastFrameTime) * 1000);
                
                if (sleepTime > 0)
                {
                    SleepMs(sleepTime); 
                }
            }
        }
    }
}