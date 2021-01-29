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
        const auto& platform = PlatformManager::Get();
        const auto& fileManager = FileManager::Get();
        auto& resourceManager = ResourceManager::Get();

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
        resourceManager.SubmitTexture("Texture/Pieces", piecesImage);
        piecesImage.FreePixels();

        FileData fontFile = fileManager.ReadEntireFile("Data/Font/NotoSans/NotoSans-Bold.ttf");
        Asset::FontAtlas fontAtlas;
        Asset::Image fontAtlasImage = fontAtlas.BuildFont(fontFile);
        resourceManager.SubmitTexture("Texture/NotoSans-Bold", fontAtlasImage);
        fontAtlasImage.FreePixels();
        
        // Due to the lack of having an actual mesh file to load,
        // all of the quads in the game are hard coded.
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
        
        Quad pieceQuads[UniquePieceCount];

        float pieceWidth = 1.0 / (UniquePieceCount / 2);

        for (auto i  = 0; i < UniquePieceCount; i++)
        {
            float minU = pieceWidth * (i % 6);
            float minV = (i < (UniquePieceCount / 2)) ? 0 : 0.5;
            float maxU = minU + pieceWidth;
            float maxV = minV + 0.5;

            pieceQuads[i] = standardQuad;
            pieceQuads[i].SetTexCoords(Vec2(minU, minV), Vec2(maxU, maxV));

            resourceManager.SubmitMesh(PieceNames[i], pieceQuads[i].Vertices, 
                                       VerticesPerQuad, sizeof(Vertex));
        }

        auto& entityManager = EntityManager::Get();
        entityManager.RegisterComponent<PieceComponent>();

        m_gameState.StartGame(PieceColor::White);
    }        
    
    Chess::~Chess()
    {

    }

    void Chess::Update()
    {
        auto& eventManager = EventManager::Get();
        eventManager.BindHandler<KeyReleased>("ForceQuit", [](const IEvent& event)
        {
            const auto& keyEvent = (const KeyReleased&)event;
            if (keyEvent.Key == InputEvent::KeyQ)
            {
                PlatformManager::ForceQuit();
            }
        });
    }

    void Chess::Render()
    {
        Render::SetShader("Textured");
        Render::FullClear(Vec4(0.17f, 0.34f, 0.68f, 1.0f));
        Render::PresentFrame();
    }
}
