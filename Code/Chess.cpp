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

        // Due to the lack of having an actual mesh file to load,
        // all of the quads in the game are hard coded.
        auto verticesPerQuad = 6;
        Quad standardQuad =
        {
            Vec3(-0.5, -0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(0, 0),
            Vec3(-0.5, 0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(0, 1),
            Vec3(0.5, 0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(1, 1),

            Vec3(0.5, 0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(1, 1),
            Vec3(0.5, -0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(1, 0),
            Vec3(-0.5, -0.5, 1.0), Vec4(0, 0, 0, 1), Vec2(0, 0),
        };

        resourceManager.SubmitMesh("StandardQuad", standardQuad.Vertices, 
                                   verticesPerQuad, sizeof(Vertex));
        
        m_pRenderObjects = std::make_unique<RenderObject[]>(UniquePieceCount);
        Quad pieceQuads[UniquePieceCount];
        const std::string pieceNames[] =
        {
            "BlackKing",
            "BlackQueen",
            "BlackBishop",
            "BlackKnight",
            "BlackRook",
            "BlackPawn",
            "WhiteKing",
            "WhiteQueen",
            "WhiteBishop",
            "WhiteKnight",
            "WhiteRook",
            "WhiteBishop",
        };

        float pieceWidth = 1.0 / (UniquePieceCount / 2);
        float pieceHeight = 0;

        for (auto i  = 0; i < UniquePieceCount; i++)
        {
            float minU = pieceWidth * i;

            if (i > (UniquePieceCount / 2))
            {
                minU = pieceWidth * (i / (UniquePieceCount / 2));
                pieceHeight = 0.5;
            }

            float maxU = minU + pieceWidth;
            float minV = pieceHeight;
            float maxV = minV + 0.5;

            pieceQuads[i] = standardQuad;
            pieceQuads[i].Vertices[0].TexCoord = Vec2(minU, minV);
            pieceQuads[i].Vertices[1].TexCoord = Vec2(minU, maxV);
            pieceQuads[i].Vertices[2].TexCoord = Vec2(maxU, maxV);
            pieceQuads[i].Vertices[3].TexCoord = Vec2(maxU, maxV);
            pieceQuads[i].Vertices[4].TexCoord = Vec2(maxU, minV);
            pieceQuads[i].Vertices[5].TexCoord = Vec2(minU, minV);

            resourceManager.SubmitMesh(pieceNames[i], pieceQuads[i].Vertices, 
                                       verticesPerQuad, sizeof(Vertex));
            m_pRenderObjects[i].AttachMesh(pieceNames[i]);
            m_pRenderObjects[i].AttachTexture("PiecesTexture");
            m_pRenderObjects[i].SetOrthographic(Vec2(), platform.GetRenderDimensions(), 0.1, 100.0);
        }

        m_gameState.StartGame(PieceColor::White);
    }        
    
    Chess::~Chess()
    {
        Render::ShutdownPipeline();
    }

    void Chess::Update()
    {
        auto& input = InputHandler::GetInstance();

        m_pRenderObjects[0].Update(Vec3(300, 300, 0), Vec3(300, 300, 1), Vec3());
    }

    void Chess::Render()
    {
        Render::FullClear(Vec4(0.17f, 0.34f, 0.68f, 1.0f));
        Render::SetShader("TexturedShader");
        m_pRenderObjects[0].Draw();
        Render::PresentFrame();
    }
}
