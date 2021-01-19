#include "Chess.h"

AppEntryPoint()
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
        Render::InitializePipeline(m_pResourceManager);

        m_gameState.StartGame(PieceColor::White); 
    }        
    
    Chess::~Chess()
    {
        Render::ShutdownPipeline();
    }

    void Chess::Update()
    {
        
    }

    void Chess::Render()
    {
        Render::FullClear(Vec4(0.2f, 0.2f, 0.2f, 1.0f));
        Render::PresentFrame();
    }
}
