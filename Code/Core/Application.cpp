#include "Application.h"

namespace Core
{
    Application::Application(const std::string& title, int32 width, int32 height)
    {
        auto& platform = PlatformManager::GetInstance(); // Initializes InputHandler.
        
        if (platform.InitializeAndCreateWindow(title.c_str(), width, height, true, true))
        {
            m_timer = Stopwatch();
            // 60 hertz is default.
            m_desiredUpdateRate = 0.0167f;
            
            // Initialize common singletons/subsystems.
            FileManager::GetInstance();
            // Initialize the renderer and render subsystems.
            Render::InitializePipeline();
            Render::ResourceManager::GetInstance();
        }
    }

    Application::~Application()
    {
        Render::ShutdownPipeline();
        PlatformManager::GetInstance().Shutdown();
    }

    void Application::Run()
    {
        auto& platform = PlatformManager::GetInstance();

        while (!platform.ShouldQuit())
        {
            m_timer.Start();

            platform.HandleSystemMessages();
            Update();
            Render();

            DeltaTime = m_timer.Stop();

            if (DeltaTime < m_desiredUpdateRate)
            {
                // Need to convert from seconds to milliseconds.
                auto sleepTime = (uint64)((m_desiredUpdateRate - DeltaTime) * 1000);
                
                if (sleepTime > 0)
                {
                    Platform::SleepMs(sleepTime); 
                }
            }
        }
    }
}