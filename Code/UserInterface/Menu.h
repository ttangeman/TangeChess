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
        Vec4 m_baseColor = Vec4(0, 0, 0, 1);
        Vec4 m_outlineColor = Vec4(1, 1, 1, 1);
        bool m_visible = false;

    public:
        void SetBaseColor(Vec4 color);
        void SetOutlineColor(Vec4 color);

        void PushRect(Vec2 position, Vec2 scale, bool outline);
        void PushButton(Vec2 position, Vec2 scale, bool outline,
                        std::function<void()> callback);

        void OnUpdate();
        void OnRender();

        void Show();
        void Hide();
        void ToggleVisibility();
    };
}