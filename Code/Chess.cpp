#include "Chess.h"
 
namespace Tange
{
    static void LoadAllShaders()
    {
        FileData defaultVertexShader = FileManager::ReadEntireFile("Shaders/Default.vs.cso");
        FileData colorFillPixelShader = FileManager::ReadEntireFile("Shaders/Fullclear.ps.cso");

        ResourceManager::SubmitShader("PixelFill",
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     colorFillPixelShader.pData.get(), colorFillPixelShader.Size);

        FileData texturePixelShader = FileManager::ReadEntireFile("Shaders/Textured.ps.cso");

        ResourceManager::SubmitShader("Textured",
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     texturePixelShader.pData.get(), texturePixelShader.Size);
    }

    static void LoadAllTextures(FontAtlas& atlas)
    {
        Image piecesImage;
        piecesImage.LoadBMP("Data/Pieces.bmp");
        ResourceManager::SubmitTexture("Texture/Pieces", piecesImage);

        FileData fontFile = FileManager::ReadEntireFile("Data/Font/NotoSans/NotoSans-Bold.ttf");
        Image fontAtlasImage = atlas.BuildFont(fontFile, "NotoSans-Bold");
        ResourceManager::SubmitTexture(atlas.FontName, fontAtlasImage);
    }

    static void InitializeChessMeshes()
    {
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

            pieceQuads[i] = Quad::DefaultQuad();
            pieceQuads[i].SetTexCoords(Vec2(minU, minV), Vec2(maxU, maxV));

            ResourceManager::SubmitMesh(PieceNames[i], pieceQuads[i].Vertices,
                                       Quad::VerticeCount, sizeof(Vertex));
        }
    }

    Chess::Chess(const std::string& title, int32 width, int32 height)
        : Application(title, width, height),
          m_menu(m_fontAtlas),
          m_gameState()
    {
        LoadAllShaders();
        LoadAllTextures(m_fontAtlas);
        InitializeChessMeshes();

        EventManager::BindHandler<KeyReleased>(0, 
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
        
        m_menu.SetBaseColor(Vec4(0, 0, 0, 0.7));
        m_menu.SetOutlineColor(Vec4(0.95, 0.89, 0.71, 0.9));

        m_menu.PushButton(Vec2(300, 300), Vec2(135, 50), 2.0, "Click me",
        []()
        {
            OutputDebugStringA("Button pressed");
        });

        m_gameState.StartGame(PieceColor::White);
    }        
    
    Chess::~Chess()
    {
    }

    void Chess::Update()
    {
        m_menu.OnUpdate();
    }

    void Chess::Render()
    {
        FullClear(Vec4(0.17f, 0.34f, 0.68f, 1.0f));

        m_menu.OnRender();

        SetShader("PixelFill");

        PresentFrame();
    }
}
