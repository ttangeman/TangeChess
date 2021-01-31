#include "Chess.h"
 
namespace Tange
{
    static void LoadAllShaders()
    {
        auto& resourceManager = ResourceManager::Get();

        FileData defaultVertexShader = FileManager::ReadEntireFile("Shaders/Default.vs.cso");
        FileData colorFillPixelShader = FileManager::ReadEntireFile("Shaders/Fullclear.ps.cso");

        resourceManager.SubmitShader("PixelFill",
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     colorFillPixelShader.pData.get(), colorFillPixelShader.Size);

        FileData texturePixelShader = FileManager::ReadEntireFile("Shaders/Textured.ps.cso");

        resourceManager.SubmitShader("Textured",
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     texturePixelShader.pData.get(), texturePixelShader.Size);
    }

    static void LoadAllTextures()
    {
        auto& resourceManager = ResourceManager::Get();

        Image piecesImage;
        piecesImage.LoadBMP("Data/Pieces.bmp");
        resourceManager.SubmitTexture("Texture/Pieces", piecesImage);

        FileData fontFile = FileManager::ReadEntireFile("Data/Font/NotoSans/NotoSans-Bold.ttf");
        FontAtlas fontAtlas;
        Image fontAtlasImage = fontAtlas.BuildFont(fontFile);
        resourceManager.SubmitTexture("Texture/NotoSans-Bold", fontAtlasImage);
    }

    static void InitializeChessMeshes()
    {
        auto& resourceManager = ResourceManager::Get();

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
        m_menu.AddWidget<Panel>(Panel(Vec2(300, 300), Vec2(180, 360), Vec4(0, 0, 0, 0.7)));

        auto& eventManager = EventManager::Get();
        eventManager.BindHandler<KeyReleased>(0, 
        [this](const IEvent& event)
        {
            const auto& keyEvent = (const KeyReleased&)event;
            if (keyEvent.Key == InputEvent::KeyQ)
            {
                PlatformManager::ForceQuit();
            }

            if (keyEvent.Key == InputEvent::KeyF11)
            {
                ToggleFullscreen();
            }

            if (keyEvent.Key == InputEvent::KeyEscape)
            {
                m_menu.ToggleVisibility();
            }
        });

        LoadAllShaders();
        LoadAllTextures();
        InitializeChessMeshes();

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
        FullClear(Vec4(0.17f, 0.34f, 0.68f, 1.0f));

        auto& eventManager = EventManager::Get();
        eventManager.Dispatch<RenderEvent>(RenderEvent());

        PresentFrame();
    }
}
