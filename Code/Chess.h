#pragma once

#include "Core/EntryPoint.h"
#include "Core/Common.h"
#include "Core/Application.h"
#include "Render/Renderer.h"
#include "Render/IResourceManager.h"
#include "GameState.h"

#include "Core/Application.cpp"
#include "Render/D3D11/D3D11Renderer.cpp"
#include "Render/D3D11/D3D11ResourceManager.cpp"
#include "GameState.cpp"

namespace Game 
{
    using namespace Render;

    class Chess : public Core::Application
    {
        public:
            
        Chess(const std::string& title, int32 width, int32 height); 
        ~Chess();

        void Update();
        void Render();
        
        private:

        std::unique_ptr<IResourceManager> m_pResourceManager;
        GameState m_gameState;
        
        Chess();
    };
}