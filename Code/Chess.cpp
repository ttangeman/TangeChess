#include "Chess.h"
 
namespace Tange
{
    static void LoadAllShaders()
    {
        FileData defaultVertexShader = FileSystem::ReadEntireFile("Shaders/Default.vs.cso");
        FileData colorFillPixelShader = FileSystem::ReadEntireFile("Shaders/Fullclear.ps.cso");

        ResourceManager::SubmitShader("PixelFill",
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     colorFillPixelShader.pData.get(), colorFillPixelShader.Size);

        FileData texturePixelShader = FileSystem::ReadEntireFile("Shaders/Textured.ps.cso");

        ResourceManager::SubmitShader("Textured",
                                     defaultVertexShader.pData.get(), defaultVertexShader.Size,
                                     texturePixelShader.pData.get(), texturePixelShader.Size);
    }

    static void LoadAllTextures(FontAtlas& atlas)
    {
        Image piecesImage;
        piecesImage.LoadBMP("Data/Pieces.bmp");
        ResourceManager::SubmitTexture("Texture/Pieces", piecesImage);

        FileData fontFile = FileSystem::ReadEntireFile("Data/Font/NotoSans/NotoSans-Bold.ttf");
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

        Vec2 drawRegion = Renderer::GetDrawRegion();
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
          m_menu(m_renderQueue),
          m_gameState()
    {
        LoadAllShaders();
        LoadAllTextures(m_fontAtlas);
        InitializeChessMeshes();

        m_boardEntity = EntityManager::RegisterEntity();
        m_boardEntity.hRender.AttachMesh("ChessBoard");
        m_boardEntity.Transform.WindowOrthographic();

        EventManager::BindHandler<KeyReleased>(0, 
        [this](const IEvent& event)
        {
            const auto& keyEvent = (const KeyReleased&)event;

            if (keyEvent.Key == InputEvent::KeyEscape)
            {
                m_menu.ToggleVisibility();
            }
        });

        m_menu.Add<GuiObject>(GuiObject {Vec2(300, 300), Vec2(240, 360), Vec4(0, 0, 0, 0.8)});
        m_menu.Add<GuiButton>(GuiButton 
        {
            Vec2(300, 400), Vec2(135, 50), 
            m_fontAtlas, "Back to Game", 12, Vec4(1, 0, 0, 1),
            [&]()
            {
                m_menu.ToggleVisibility();
            }
        });

#if 0
        m_menu.PushPanel(Vec2(300, 300), Vec2(240, 360), 2.0);

        m_menu.PushButton(Vec2(300, 400), Vec2(135, 50), 2.0, "Back to Game",
        [&]()
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
#endif
        m_gameState.StartGame(PieceColor::White);
    }        
    
    Chess::~Chess()
    {
    }

    void Chess::Update()
    {
        for (auto entity : m_gameState.BoardState)
        {
            if (EntityManager::HasComponent<WorldTransform>(entity))
            {
                auto& worldP = EntityManager::GetComponent<WorldTransform>(entity);
                entity.Transform.Update(worldP.Position, worldP.Scale, worldP.Orientation);
            }
        }

        m_menu.Update(m_dTime);
    }

    void Chess::Render()
    {
        Renderer::FullClear(Vec4(0.17f, 0.34f, 0.68f, 1.0f));

        m_renderQueue.Submit("PixelFill", m_boardEntity.hRender, m_boardEntity.Transform);

        for (auto entity : m_gameState.BoardState)
        {
            if (entity.IsValid())
            {
                m_renderQueue.Submit("Textured", entity.hRender, entity.Transform);
            }
        }

        m_menu.Render();

        Renderer::PresentFrame();
    }
}
