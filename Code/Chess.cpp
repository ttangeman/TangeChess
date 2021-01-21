#include "Chess.h"

AppEntryPoint()
{
    auto* pChess = new Game::Chess("Chess", 600, 600); 
    pChess->Run();
    delete pChess;

    return 0;
}

namespace Game
{
    Chess::Chess(const std::string& title, int32 width, int32 height)
        : Core::Application(title, width, height)
    {
        Render::InitializePipeline();

        auto& resourceManager = ResourceManager::GetInstance();
        
        Vertex vertData[] =
        {
            {Vec3(0.0f, 0.5f, 0.0f), Vec4(), Vec2()},
            {Vec3(0.5f, -0.5f, 0.0f), Vec4(), Vec2()},
            {Vec3(-0.5f, -0.5f, 0.0f), Vec4(), Vec2()},
        };
        
        std::vector<Vertex> verts;
        for (auto v : vertData)
            verts.push_back(v);
        
        resourceManager.SubmitMesh("Test", verts);

        RenderObject robj;
        
        // TODO: File loading and shaders.

        m_gameState.StartGame(PieceColor::White); 
    }        
    
    Chess::~Chess()
    {
        Render::ShutdownPipeline();
    }

    void Chess::Update()
    {
        
    }

    void Chess::Render()
    {
        Render::FullClear(Vec4(0.2f, 0.2f, 0.2f, 1.0f));
        Render::PresentFrame();
    }
}
