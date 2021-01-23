#include "Chess.h"

MAIN_ENTRY_POINT()
{
    auto* pChess = new Game::Chess("Chess", 600, 600); 
    pChess->Run();
    delete pChess;

    return 0;
}

namespace Game
{
    RenderObject triangleObject;

    Chess::Chess(const std::string& title, int32 width, int32 height)
        : Core::Application(title, width, height)
    {
        Render::InitializePipeline();

        const auto& fileManager = FileManager::GetInstance();
        auto& resourceManager = ResourceManager::GetInstance();

        FileData vertexShaderData = fileManager.ReadEntireFile("Shaders/Default.vs.cso");
        FileData pixelShaderData = fileManager.ReadEntireFile("Shaders/Fullclear.ps.cso");

        resourceManager.SubmitShader("DefaultShader", 
                                     vertexShaderData.pData.get(), vertexShaderData.Size,
                                     pixelShaderData.pData.get(), pixelShaderData.Size);

        Vertex triangleData[] =
        {
            {Vec3(0.0f, 0.5f, 0.0f), Vec4(1, 0, 0, 1), Vec2()},
            {Vec3(0.5f, -0.5f, 0.0f), Vec4(0, 1, 0, 1), Vec2()},
            {Vec3(-0.5f, -0.5f, 0.0f), Vec4(0, 0, 1, 1), Vec2()},
        };
        
        resourceManager.SubmitMesh("Triangle", triangleData, ARRAY_LENGTH(triangleData), sizeof(Vertex));
        triangleObject.AttachMesh("Triangle");

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
        Render::SetShader("DefaultShader");
        triangleObject.Draw();
        Render::PresentFrame();
    }
}
