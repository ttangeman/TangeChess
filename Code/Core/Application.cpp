#include "Application.h"

namespace Core
{
    Application::Application(const std::string& title, int32 width, int32 height)
    {
        auto& platform = PlatformManager::Get();

        // NOTE: The platform manager initializes the event manager
        // and registers input events for us.
        if (platform.InitializeAndCreateWindow(title.c_str(), width, height, true, true))
        {
            m_timer = Stopwatch();
            // 60 hertz is default.
            m_desiredUpdateRate = 0.0167f;
            
            // Initialize common singletons/subsystems.
            Platform::FileManager::Get();
            Render::InitializePipeline();
            Render::ResourceManager::Get();

            // Register common entity components.
            auto& entityManager = ECS::EntityManager::Get();
            entityManager.RegisterComponent<Render::Drawable>();
            entityManager.RegisterComponent<Render::Transformable>();
        }
    }

    Application::~Application()
    {
        Render::ShutdownPipeline();
        PlatformManager::Get().Shutdown();
    }

    void Application::Run()
    {
        auto& platform = PlatformManager::Get();

        while (!platform.ShouldQuit())
        {
            m_timer.Start();

            platform.DispatchSystemMessages();
            Update();
            Render();

            LastFrameTime = m_timer.Stop();

            if (LastFrameTime < m_desiredUpdateRate)
            {
                // Need to convert from seconds to milliseconds.
                auto sleepTime = (uint64)((m_desiredUpdateRate - LastFrameTime) * 1000);
                
                if (sleepTime > 0)
                {
                    Platform::SleepMs(sleepTime); 
                }
            }
        }
    }
}