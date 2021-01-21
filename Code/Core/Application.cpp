#include "Application.h"

namespace Core
{
    Application::Application(const std::string& title, int32 width, int32 height)
    {
        auto& platform = PlatformManager::GetInstance();
        
        if (platform.InitializeAndCreateWindow(title.c_str(), width, height, true, true))
        {
            m_timer = Stopwatch();
            // 60 hertz is default.
            m_desiredUpdateRate = 0.0167f;
        }
    }

    Application::~Application()
    {
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

            LastTimeStep = m_timer.Stop();

            if (LastTimeStep < m_desiredUpdateRate)
            {
                // Need to convert from seconds to milliseconds.
                auto sleepTime = (uint64)((m_desiredUpdateRate - LastTimeStep) * 1000);
                
                if (sleepTime > 0)
                {
                    Platform::SleepMs(sleepTime); 
                }
            }
        }
    }
}