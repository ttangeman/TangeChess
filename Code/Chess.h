#pragma once

#include "Core/EntryPoint.h"
#include "Core/Common.h"
#include "Core/Application.h"
#include "Render/Renderer.h"
#include "Render/RenderObject.h"
#include "Render/ResourceManager.h"
#include "GameState.h"

#include "Core/Application.cpp"
#include "Render/D3D11/D3D11Renderer.cpp"
#include "Render/D3D11/D3D11RenderObject.cpp"
#include "Render/D3D11/D3D11ResourceManager.cpp"
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
        std::unique_ptr<RenderObject> m_pQuad;
    };
}