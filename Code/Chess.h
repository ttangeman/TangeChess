#pragma once

#include "Core/Application.h"
#include "GameState.h"

#include "Core/Application.cpp"
#include "GameState.cpp"

namespace Tange 
{
    class Chess : public Application
    {
        GameState m_gameState;
        Menu m_menu;

    public:    
        Chess() = delete;
        Chess(const std::string& title, int32 width, int32 height); 
        ~Chess();

        void Update() override;
        void Render() override;
    };
}