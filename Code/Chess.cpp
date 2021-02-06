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
        Quad pieceQuads[GameState::UniquePieceCount];

        float pieceWidth = 1.0 / (GameState::UniquePieceCount / 2);

        // Push the piece meshes.
        for (auto i = 0; i < GameState::UniquePieceCount; i++)
        {
            // NOTE: The piece texture is a 6x2 atlas.
            float minU = pieceWidth * (i % 6);
            float minV = (i < (GameState::UniquePieceCount / 2)) ? 0 : 0.5;
            float maxU = minU + pieceWidth;
            float maxV = minV + 0.5;

            pieceQuads[i] = Quad::Default();
            pieceQuads[i].SetTexCoords(Vec2(minU, minV), Vec2(maxU, maxV));

            ResourceManager::SubmitMesh(g_PieceNames[i], pieceQuads[i].Vertices,
                                        Quad::VerticeCount, sizeof(Vertex));
        }

        // Push the board mesh.
        Vec4 colors[] = 
        {
            {0.71, 0.53, 0.39, 1.0}, // Dark
            {0.94, 0.85, 0.71, 1.0}, // Light
        };

        Vec2 drawRegion = GetDrawRegion();
        auto squareDim = Vec2(drawRegion.Width / GameState::ColCount,
                              drawRegion.Height / GameState::RowCount);

        Quad boardQuads[GameState::SquareCount];

        for (auto row = 0; row < GameState::RowCount; row++)
        { 
            auto minP = Vec2(0.0, squareDim.Height * row);
            auto maxP = Vec2(squareDim.Width, squareDim.Height * (row + 1));

            for (auto col = 0; col < GameState::ColCount; col++)
            {
                auto color = colors[(col + row) % 2];
                auto index = (row * GameState::ColCount) + col;

                boardQuads[index] = Quad::CreatePreTransformed(minP, maxP, color);

                minP.X += squareDim.Width;
                maxP.X += squareDim.Width;
            }
        }

        ResourceManager::SubmitMesh("ChessBoard", reinterpret_cast<Vertex*>(boardQuads),
                                    Quad::VerticeCount * GameState::SquareCount, sizeof(Vertex));
    }

    Chess::Chess(const std::string& title, int32 width, int32 height)
        : Application(title, width, height),
          m_menu(m_fontAtlas),
          m_gameState()
    {
        LoadAllShaders();
        LoadAllTextures(m_fontAtlas);
        InitializeChessMeshes();

        m_boardEntity = EntityManager::RegisterEntity();
        
        auto& drawable = EntityManager::AttachComponent<Drawable>(m_boardEntity);
        drawable.AttachMesh("ChessBoard");

        auto& transform = EntityManager::AttachComponent<Transformable>(m_boardEntity);
        transform.SetOrthographic(Vec2(), GetDrawRegion(), 0.1, 100.0);
        transform.OnUpdate();

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
        
        m_menu.SetBaseColor(Vec4(0, 0, 0, 0.9));
        m_menu.SetOutlineColor(Vec4(0.95, 0.89, 0.71, 0.9));
        m_menu.SetTextColor(Vec4(0.95, 0.89, 0.71, 0.95));

        m_menu.PushPanel(Vec2(300, 300), Vec2(240, 360), 2.0);

        m_menu.PushButton(Vec2(300, 400), Vec2(135, 50), 2.0, "Back to Game",
        [this]()
        {
            m_menu.ToggleVisibility();
        });

        m_menu.PushButton(Vec2(300, 300), Vec2(135, 50), 2.0, "Options",
        []()
        {
        });

        m_menu.PushButton(Vec2(300, 200), Vec2(135, 50), 2.0, "Quit",
        []()
        {
            PlatformManager::ForceQuit();
        });

        m_gameState.StartGame(PieceColor::White);
    }        
    
    Chess::~Chess()
    {
    }

    void Chess::Update()
    {
        for (auto entity : m_gameState.BoardState)
        {
            if (entity.IsValid())
            {
                auto& transform = EntityManager::GetComponent<Transformable>(entity);
                transform.OnUpdate();
            }
        }

        m_menu.OnUpdate();
    }

    void Chess::Render()
    {
        FullClear(Vec4(0.17f, 0.34f, 0.68f, 1.0f));

        SetShader("PixelFill");

        // TODO: Clearly a lot of this code is repeated.
        // I would just ask the entity manager for all drawables/transformables,
        // but the menu does not want to always be drawn, so I need a way
        // to indicate that. Also, the shader needs to be set.
        auto& boardTransform = EntityManager::GetComponent<Transformable>(m_boardEntity);
        boardTransform.OnRender();

        auto& boardDrawable = EntityManager::GetComponent<Drawable>(m_boardEntity);
        boardDrawable.OnRender();

        SetShader("Textured");

        for (auto entity : m_gameState.BoardState)
        {
            if (entity.IsValid())
            {
                auto& transform = EntityManager::GetComponent<Transformable>(entity);
                transform.OnRender();

                auto& drawable = EntityManager::GetComponent<Drawable>(entity);
                drawable.OnRender();
            }
        }

        m_menu.OnRender();

        PresentFrame();
    }
}
