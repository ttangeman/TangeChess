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
    Vec3 position;
    Vec3 scale;
    Vec3 rotation;

    Chess::Chess(const std::string& title, int32 width, int32 height)
        : Core::Application(title, width, height)
    {
        Render::InitializePipeline();

        const auto& platform = PlatformManager::GetInstance();
        const auto& fileManager = FileManager::GetInstance();
        auto& resourceManager = ResourceManager::GetInstance();

        FileData defaultVertexShader = fileManager.ReadEntireFile("Shaders/Default.vs.cso");
        FileData colorFillPixelShader = fileManager.ReadEntireFile("Shaders/Fullclear.ps.cso");
        FileData texturePixelShader = fileManager.ReadEntireFile("Shaders/Textured.ps.cso");

        resourceManager.SubmitShader("ColorFillShader", 
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     colorFillPixelShader.pData.get(), colorFillPixelShader.Size);
        resourceManager.SubmitShader("TexturedShader",
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     texturePixelShader.pData.get(), texturePixelShader.Size);

        Image piecesImage;
        piecesImage.LoadBMP("Data/Pieces.bmp");
        resourceManager.SubmitTexture("PiecesTexture", piecesImage);
        piecesImage.FreePixels();

        Vertex quadData[] =
        {
            {Vec3(-0.5, -0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(0, 0)},
            {Vec3(-0.5, 0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(0, 1)},
            {Vec3(0.5, 0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(1, 1)},

            {Vec3(0.5, 0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(1, 1)},
            {Vec3(0.5, -0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(1, 0)},
            {Vec3(-0.5, -0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(0, 0)},
        };

        resourceManager.SubmitMesh("Quad", quadData, ARRAY_LENGTH(quadData), sizeof(Vertex));

        m_pQuad = std::make_unique<RenderObject>();
        m_pQuad->AttachMesh("Quad");
        m_pQuad->AttachTexture("PiecesTexture");
        m_pQuad->SetOrthographic(Vec2(0, 0), platform.GetRenderDimensions(), 0.1f, 100.0f);

        position = Vec3(300, 300, 0);
        scale = Vec3(piecesImage.Width / 2.0f, piecesImage.Height / 2.0f, 1);
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
            position.X += 5.0f;
        }

        if (input.IsCurrentlyPressed(InputEvent::MoveLeft))
        {
            position.X -= 5.0f;
        }

        m_pQuad->Update(position, scale, rotation);
    }

    void Chess::Render()
    {
        Render::FullClear(Vec4(0.17f, 0.34f, 0.68f, 1.0f));
        Render::SetShader("TexturedShader");
        m_pQuad->Draw();
        Render::PresentFrame();
    }
}
