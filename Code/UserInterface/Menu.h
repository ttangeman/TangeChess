#pragma once

#include "Core/Common.h"
#include "Entity/EntityManager.h"
#include "UserInterface/GuiComponents.h"
#include "Render/RenderQueue.h"
#include "Asset/FontAtlas.h"

namespace Tange
{
    // TODO: Instead of using window coordinates, the Menu could always
    // default to a (500, 500) orthographic coordinate system instead and
    // map mouse positions into that system. This would ensure that the GUI is scaled
    // right on all aspect ratios.
    class Menu
    {
        const RenderQueue& m_renderQueue;
        const FontAtlas& m_atlas;

        Vec4 m_baseColor = Vec4(0, 0, 0, 1);
        Vec4 m_outlineColor = Vec4(1, 1, 1, 1);
        Vec4 m_textColor = Vec4(1, 1, 1, 1);

        bool m_visible = false;

    public:
        Menu(const RenderQueue& renderQueue, const FontAtlas& atlas);
        ~Menu();

        void SetBaseColor(Vec4 color);
        void SetOutlineColor(Vec4 color);
        void SetTextColor(Vec4 color);

        void PushText(const std::string& text, Vec2 position, float pixelHeight);
        void PushPanel(Vec2 position, Vec2 scale, float outlineThickness);
        void PushButton(Vec2 position, Vec2 scale, float outlineThickness,
                        const std::string& text, const std::function<void()>& callback);

        void Update(float timeStep);
        void Render();

        void Show();
        void Hide();
        void ToggleVisibility();
    };
}