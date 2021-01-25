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
    std::unique_ptr<RenderObject> testGlyph;

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

        resourceManager.SubmitShader("PixelFill", 
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     colorFillPixelShader.pData.get(), colorFillPixelShader.Size);
        resourceManager.SubmitShader("Textured",
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     texturePixelShader.pData.get(), texturePixelShader.Size);

        Asset::Image piecesImage;
        piecesImage.LoadBMP("Data/Pieces.bmp");
        resourceManager.SubmitTexture("PiecesTexture", piecesImage);
        piecesImage.FreePixels();

        FileData fontFile = fileManager.ReadEntireFile("Data/Font/NotoSans/NotoSans-Bold.ttf");
        Asset::FontAtlas fontAtlas;
        Asset::Image fontAtlasImage = fontAtlas.BuildFont(fontFile, 128);
        resourceManager.SubmitTexture("FontTexture", fontAtlasImage);
        fontAtlasImage.FreePixels();
       
        // Due to the lack of having an actual mesh file to load,
        // all of the quads in the game are hard coded.
        const int32 VerticesPerQuad = 6;
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
                                   VerticesPerQuad, sizeof(Vertex));

        const auto& a = fontAtlas.LookupGlyphInfo('a');

        Quad fontQuad = standardQuad;
        fontQuad.SetTexCoords(a.MinUV, a.MaxUV);

        resourceManager.SubmitMesh("TestQuad", fontQuad.Vertices, VerticesPerQuad, sizeof(Vertex));

        testGlyph = std::make_unique<RenderObject>();
        testGlyph->AttachMesh("TestQuad");
        testGlyph->AttachTexture("FontTexture");
        testGlyph->SetOrthographic(Vec2(), platform.GetRenderDimensions(), 0.1, 100.0);
        
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

        for (auto i  = 0; i < UniquePieceCount; i++)
        {
            float minU = pieceWidth * (i % 6);
            float minV = (i < (UniquePieceCount / 2)) ? 0 : 0.5;
            float maxU = minU + pieceWidth;
            float maxV = minV + 0.5;

            pieceQuads[i] = standardQuad;
            pieceQuads[i].SetTexCoords(Vec2(minU, minV), Vec2(maxU, maxV));

            resourceManager.SubmitMesh(pieceNames[i], pieceQuads[i].Vertices, 
                                       VerticesPerQuad, sizeof(Vertex));
                                       
            m_pRenderObjects[i].AttachMesh(pieceNames[i]);
            m_pRenderObjects[i].AttachTexture("PiecesTexture");
            // This projection matrix evenly distributes 8 pieces among a row
            // given the interval [1, 8] for positions. The 0.5 acts as an offset
            // from the edge of the screen.
            m_pRenderObjects[i].SetOrthographic(Vec2(0.5, 0.5), Vec2(8.5, 8.5), 0.1, 100.0);
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

        for (auto i = 0; i < ColCount; i++)
        {
            m_pRenderObjects[i].Update(Vec3(i + 1, 1, 0), Vec3(1, 1, 1), Vec3());
        }

        testGlyph->Update(Vec3(300, 300, 1), Vec3(50, 50, 0), Vec3());
    }

    void Chess::Render()
    {
        Render::FullClear(Vec4(0.17f, 0.34f, 0.68f, 1.0f));
        Render::SetShader("Textured");

        for (auto i = 0; i < ColCount; i++)
        {
            m_pRenderObjects[i].Draw();
        }

        testGlyph->Draw();
        Render::PresentFrame();
    }
}
