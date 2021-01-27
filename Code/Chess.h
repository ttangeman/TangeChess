#pragma once

#include "Core/Application.h"
#include "GameState.h"

#include "Core/Application.cpp"
#include "GameState.cpp"

namespace Game 
{
    using namespace Platform;
    using namespace Render;

    class Chess : public Core::Application
    {
        public:
            
        Chess() = delete;
        Chess(const std::string& title, int32 width, int32 height); 
        ~Chess();

        void Update();
        void Render();
        
        private:

        GameState m_gameState;
        // TODO: Push these to the render queue, rather than maintaining an array of these
        // in the game state.
        std::unique_ptr<RenderObject[]> m_pRenderObjects;
    };
}