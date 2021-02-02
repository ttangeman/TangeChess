#pragma once

#include "Core/Common.h"
#include "Entity/EntityManager.h"
#include "Render/Drawable.h"
#include "Render/Transformable.h"
#include "UserInterface/GuiComponents.h"

namespace Tange
{
    class Menu
    {
        std::vector<Entity> m_entities;
        bool m_visible = false;

    public:
        void AddPanel(Vec2 position, Vec2 scale, Vec4 color);
        void AddButton(Vec2 position, Vec2 scale, Vec4 color, 
                       std::function<void()> callback);

        void OnUpdate();
        void OnRender();

        void Show();
        void Hide();
        void ToggleVisibility();
    };
}