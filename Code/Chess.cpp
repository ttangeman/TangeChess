#include "Chess.h"
 
namespace Tange
{
    static void LoadAllShaders()
    {
        auto &resourceManager = ResourceManager::Get();

        FileData defaultVertexShader = FileManager::ReadEntireFile("Shaders/Default.vs.cso");
        FileData colorFillPixelShader = FileManager::ReadEntireFile("Shaders/Fullclear.ps.cso");
        FileData texturePixelShader = FileManager::ReadEntireFile("Shaders/Textured.ps.cso");

        resourceManager.SubmitShader("PixelFill",
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     colorFillPixelShader.pData.get(), colorFillPixelShader.Size);
        resourceManager.SubmitShader("Textured",
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     texturePixelShader.pData.get(), texturePixelShader.Size);
    }

    static void LoadAllTextures()
    {
        auto &resourceManager = ResourceManager::Get();

        Image piecesImage;
        piecesImage.LoadBMP("Data/Pieces.bmp");
        resourceManager.SubmitTexture("Texture/Pieces", piecesImage);
        piecesImage.FreePixels();

        FileData fontFile = FileManager::ReadEntireFile("Data/Font/NotoSans/NotoSans-Bold.ttf");
        FontAtlas fontAtlas;
        Image fontAtlasImage = fontAtlas.BuildFont(fontFile);
        resourceManager.SubmitTexture("Texture/NotoSans-Bold", fontAtlasImage);
        fontAtlasImage.FreePixels();
    }

    static void InitializeChessMeshes()
    {
        auto &resourceManager = ResourceManager::Get();

        // Due to the lack of having an actual mesh file to load,
        // all of the quads in the game are hard coded.
        Quad pieceQuads[UniquePieceCount];

        float pieceWidth = 1.0 / (UniquePieceCount / 2);

        for (auto i = 0; i < UniquePieceCount; i++)
        {
            float minU = pieceWidth * (i % 6);
            float minV = (i < (UniquePieceCount / 2)) ? 0 : 0.5;
            float maxU = minU + pieceWidth;
            float maxV = minV + 0.5;

            pieceQuads[i] = DefaultQuad;
            pieceQuads[i].SetTexCoords(Vec2(minU, minV), Vec2(maxU, maxV));

            resourceManager.SubmitMesh(PieceNames[i], pieceQuads[i].Vertices,
                                       Quad::VerticeCount, sizeof(Vertex));
        }
    }

    Chess::Chess(const std::string& title, int32 width, int32 height)
        : Application(title, width, height),
          m_gameState()
    {
        auto &eventManager = EventManager::Get();

        eventManager.BindHandler<KeyPressed>(0, 
        [](const IEvent &event)
        {
            const auto &keyEvent = (const KeyPressed &)event;
            if (keyEvent.Key == InputEvent::KeyEscape)
            {
                PlatformManager::ForceQuit();
            }

            if (keyEvent.Key == InputEvent::KeyF11)
            {
                ToggleFullscreen();
            }
        });

        LoadAllShaders();
        LoadAllTextures();
        InitializeChessMeshes();

        test.AddPanel(Vec2(300, 300), Vec2(100, 100));

        m_gameState.StartGame(PieceColor::White);
    }        
    
    Chess::~Chess()
    {
    }

    void Chess::Update()
    {
        auto& eventManager = EventManager::Get();
        eventManager.Dispatch<UpdateEvent>(UpdateEvent());
    }

    void Chess::Render()
    {
        SetShader("PixelFill");
        FullClear(Vec4(0.17f, 0.34f, 0.68f, 1.0f));

        auto& eventManager = EventManager::Get();
        eventManager.Dispatch<RenderEvent>(RenderEvent());

        PresentFrame();
    }
}
