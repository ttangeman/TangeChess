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
    Chess::Chess(const std::string& title, int32 width, int32 height)
        : Core::Application(title, width, height)
    {
        Render::InitializePipeline();

        const auto& platform = PlatformManager::GetInstance();
        const auto& fileManager = FileManager::GetInstance();
        auto& resourceManager = ResourceManager::GetInstance();

        FileData defaultVertexShaderData = fileManager.ReadEntireFile("Shaders/Default.vs.cso");
        FileData colorFillPixelShaderData = fileManager.ReadEntireFile("Shaders/Fullclear.ps.cso");
        FileData texturePixelShaderData = fileManager.ReadEntireFile("Shaders/Textured.ps.cso");

        resourceManager.SubmitShader("ColorFillShader", 
                                     defaultVertexShaderData.pData.get(), defaultVertexShaderData.Size,
                                     colorFillPixelShaderData.pData.get(), colorFillPixelShaderData.Size);
        resourceManager.SubmitShader("TexturedShader",
                                     defaultVertexShaderData.pData.get(), defaultVertexShaderData.Size,
                                     texturePixelShaderData.pData.get(), texturePixelShaderData.Size);
        /*
        Vertex triangleData[] =
        {
            {Vec3(100.0f, 50.0f, 1.0f), Vec4(1, 0, 0, 1), Vec2()},
            {Vec3(300.0f, 350.0f, 1.0f), Vec4(0, 1, 0, 1), Vec2()},
            {Vec3(500.0f, 50.0f, 1.0f), Vec4(0, 0, 1, 1), Vec2()},
        };
        */
       Vertex triangleData[] =
       {
           {Vec3(-0.5f, -0.5f, 1.0f), Vec4(1, 0, 0, 1), Vec2()},
           {Vec3(0.0f, 0.5f, 1.0f), Vec4(0, 1, 0, 1), Vec2()},
           {Vec3(0.5f, -0.5f, 1.0f), Vec4(0, 0, 1, 1), Vec2()},
       };

        resourceManager.SubmitMesh("Triangle", triangleData, ARRAY_LENGTH(triangleData), sizeof(Vertex));

        m_pTriangle = std::make_unique<RenderObject>();
        m_pTriangle->AttachMesh("Triangle");
        m_pTriangle->SetOrthographic(Vec2(0, 0), platform.GetRenderDimensions(), 0.1f, 100.0f);
        m_pTriangle->Scale(Vec3(200, 200, 1));
        m_pTriangle->Translate(Vec3(300, 300, 0));

        m_gameState.StartGame(PieceColor::White); 
    }        
    
    Chess::~Chess()
    {
        Render::ShutdownPipeline();
    }

    void Chess::Update()
    {
        auto& input = InputHandler::GetInstance();

        if (input.IsCurrentlyPressed(InputEvent::MoveRight))
        {
            m_pTriangle->Translate(Vec3(5.0f, 0, 0));
        }

        if (input.IsCurrentlyPressed(InputEvent::MoveLeft))
        {
            m_pTriangle->Translate(Vec3(-5.0f, 0, 0));
        }
        
        m_pTriangle->Translate(Vec3(-300, -300, 0));
        m_pTriangle->Rotate(Vec3(0, 0, 3));
        m_pTriangle->Translate(Vec3(300, 300, 0));
        
        m_pTriangle->Update();
    }

    void Chess::Render()
    {
        Render::FullClear(Vec4(0.2f, 0.2f, 0.2f, 1.0f));
        Render::SetShader("ColorFillShader");
        m_pTriangle->Draw();
        Render::PresentFrame();
    }
}
