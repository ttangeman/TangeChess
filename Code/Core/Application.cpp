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

            // TODO: Need a way to update all orthographic entities
            // with the new width and height.
            EventManager::BindHandler<WindowResized>(0, 
            [](const IEvent& event)
            {
                const auto& resizeData = static_cast<const WindowResized&>(event);
                ResizeWindow(resizeData.DesiredWidth, resizeData.DesiredHeight);
            });

            // Register common entity components.
            EntityManager::RegisterComponent<WorldTransform>();
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

            m_lastFrameTime = m_timer.Stop();

            if (m_lastFrameTime < m_desiredUpdateRate)
            {
                m_dTime = m_desiredUpdateRate;
                
                // Need to convert from seconds to milliseconds.
                auto sleepTime = (uint64)((m_desiredUpdateRate - m_lastFrameTime) * 1000);
                
                if (sleepTime > 0)
                {
                    SleepMs(sleepTime); 
                }
            }
            else
            {
                m_dTime = m_lastFrameTime;
            }
        }
    }
}