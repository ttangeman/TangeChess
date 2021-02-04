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
        const FontAtlas& m_atlas;
        Vec4 m_baseColor = Vec4(0, 0, 0, 1);
        Vec4 m_outlineColor = Vec4(1, 1, 1, 1);
        Vec4 m_textColor = Vec4(1, 1, 1, 1);
        bool m_visible = false;

    public:
        Menu(const FontAtlas& atlas);
        ~Menu();

        void SetBaseColor(Vec4 color);
        void SetOutlineColor(Vec4 color);
        void SetTextColor(Vec4 color);

        void PushPanel(Vec2 position, Vec2 scale, float outlineThickness);
        void PushButton(Vec2 position, Vec2 scale, float outlineThickness,
                        const std::string& text, std::function<void()> callback);

        void OnUpdate();
        void OnRender();

        void Show();
        void Hide();
        void ToggleVisibility();
    };
}