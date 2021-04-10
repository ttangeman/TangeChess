#pragma once

#include "UserInterface/GuiObject.h"

namespace Tange
{
    class GuiOutline
    {
        RenderHandle m_hRenders[4];
        Transform m_transforms[4];
        Vec2 m_positions[4];
        Vec2 m_scales[4];
        float m_outlineThickness;
        Vec4 m_outlineColor;
        bool m_visible = false;
        
    public:
        GuiOutline(Vec2 centerP, Vec2 dimensions, float thickness, 
                   Vec4 color = Vec4(1, 1, 1, 1))
            : m_outlineThickness(thickness), m_outlineColor(color)
        {
            Vec2 radius = Vec2(dimensions.Width / 2.0, dimensions.Height / 2.0);
            
            m_positions[0] = Vec2(centerP.X - radius.X, centerP.Y);
            m_positions[1] = Vec2(centerP.X, centerP.Y + radius.Y);
            m_positions[2] = Vec2(centerP.X + radius.X, centerP.Y);
            m_positions[3] = Vec2(centerP.X, centerP.Y - radius.Y);

            m_scales[0] = Vec2(thickness, dimensions.Y + thickness);
            m_scales[1] = Vec2(dimensions.X + thickness, thickness);
            m_scales[2] = Vec2(thickness, dimensions.Y + thickness);
            m_scales[3] = Vec2(dimensions.X + thickness, thickness);

            for (auto i = 0; i < 4; i++)
            {
                m_hRenders[i].AttachMesh("DefaultQuad");
                m_hRenders[i].SetColor(m_outlineColor);
                m_transforms[i].WindowOrthographic();
            }

            // TODO: Get a proper id from event manager!
            EventManager::BindHandler<WindowResized>(1,
            [this](const IEvent& event)
            {
                const auto& resizeEvent = static_cast<const WindowResized&>(event);

                for (auto i = 0; i < 4; i++)
                {
                    auto tScale = Vec2(m_scales[i].Width / resizeEvent.CurrentWidth,
                                       m_scales[i].Height / resizeEvent.CurrentHeight);
                    m_scales[i] = Vec2(resizeEvent.DesiredWidth * tScale.Width,
                                       resizeEvent.DesiredHeight * tScale.Height);
                    
                    auto tPosition = Vec2(m_positions[i].Width / resizeEvent.CurrentWidth,
                                          m_positions[i].Height / resizeEvent.CurrentHeight);
                    m_positions[i] = Vec2(resizeEvent.DesiredWidth * tPosition.Width,
                                          resizeEvent.DesiredHeight * tPosition.Height);
                }
            });
        }

        GuiOutline(const GuiConfig& config, Vec2 centerP, Vec2 dimensions)
            : GuiOutline(centerP, dimensions, config.OutlineThickness, config.OutlineColor)
        {
        }

        void Destroy()
        {
            EventManager::DetachHandler<WindowResized>(1);
        }

        void Update(float timeStep)
        {
            for (auto i = 0; i < 4; i++)
            {
                m_transforms[i].Update(Vec3(m_positions[i], 1),
                                       Vec3(m_scales[i], 1),
                                       Vec3(0, 0, 0));
            }
        }

        void Render(const RenderQueue& queue)
        {
            if (m_visible)
            {
                for (auto i = 0; i < 4; i++)
                {
                    queue.Submit("PixelFill", m_hRenders[i], m_transforms[i]);
                }
            }
        }

        void Show()
        {
            m_visible = true;
        }

        void Hide()
        {
            m_visible = false;
        }

        void ToggleVisibility()
        {
            if (!m_visible)
            {
                Show();
            }
            else
            {
                Hide();
            }
        }
    };
}