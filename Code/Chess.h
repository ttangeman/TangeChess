#pragma once

#include "Core/Application.h"
#include "GameState.h"

#include "Core/Application.cpp"
#include "GameState.cpp"

namespace Tange 
{
    class Chess : public Application
    {
        RenderQueue m_renderQueue;
        GameState m_gameState;
        FontAtlas m_fontAtlas;
        GuiMenu m_menu;
        Entity m_boardEntity;

    public:    
        Chess() = delete;
        Chess(const std::string& title, int32 width, int32 height); 
        ~Chess();

        void Update() override;
        void Render() override;
    };
}