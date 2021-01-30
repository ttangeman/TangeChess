#include "Application.h"

namespace Tange
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
            FileManager::Get();
            
            // Initialize the renderer.
            IntializeRendererPipeline();
            auto &resourceManager = ResourceManager::Get();

            resourceManager.SubmitMesh("DefaultQuad", DefaultQuad.Vertices,
                                       Quad::VerticeCount, sizeof(Vertex));

            // Register common entity components and events.
            auto &entityManager = EntityManager::Get();
            entityManager.RegisterComponent<Drawable>();
            entityManager.RegisterComponent<Transformable>();
            entityManager.RegisterComponent<GuiComponent>();

            auto& eventManager = EventManager::Get();
            eventManager.RegisterEvent<UpdateEvent>();
            eventManager.RegisterEvent<RenderEvent>();
        }
    }

    Application::~Application()
    {
        ShutdownRendererPipeline();
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
                    SleepMs(sleepTime); 
                }
            }
        }
    }
}