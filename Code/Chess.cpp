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
    Vec3 pos;
    Vec3 scale;
    Vec3 rotation;

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

        FileData piecesTexture = fileManager.ReadEntireFile("Assets/Pieces.bmp");

        // TODO: BMP loader.
        
        Vertex quadData[] =
        {
            {Vec3(-0.5f, -0.5f, 1.0f), Vec4(0, 0, 0, 1), Vec2()},
            {Vec3(-0.5f, 0.5f, 1.0f), Vec4(0.2, 0.2, 0.2, 1), Vec2()},
            {Vec3(0.5f, 0.5f, 1.0f), Vec4(0.7, 0.7, 0.7, 1), Vec2()},

            {Vec3(0.5f, 0.5f, 1.0f), Vec4(0.7, 0.7, 0.7, 1), Vec2()},
            {Vec3(0.5f, -0.5f, 1.0f), Vec4(0.2, 0.2, 0.2, 1), Vec2()},
            {Vec3(-0.5f, -0.5f, 1.0f), Vec4(0, 0, 0, 1), Vec2()},
        };

        resourceManager.SubmitMesh("Quad", quadData, ARRAY_LENGTH(quadData), sizeof(Vertex));

        m_pQuad = std::make_unique<RenderObject>();
        m_pQuad->AttachMesh("Quad");
        m_pQuad->SetOrthographic(Vec2(0, 0), platform.GetRenderDimensions(), 0.1f, 100.0f);

        pos = Vec3(300, 300, 0);
        scale = Vec3(200, 200, 1);
        rotation = Vec3();

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
            pos.X += 5.0f;
        }

        if (input.IsCurrentlyPressed(InputEvent::MoveLeft))
        {
            pos.X -= 5.0f;
        }
        
        rotation.Z += 3.0f;

        m_pQuad->Update(pos, scale, rotation);
    }

    void Chess::Render()
    {
        Render::FullClear(Vec4(0.17f, 0.34f, 0.68f, 1.0f));
        Render::SetShader("ColorFillShader");
        m_pQuad->Draw();
        Render::PresentFrame();
    }
}
