#pragma once

#include "Core/Common.h"
#include "Render/Renderer.h"
#include "Render/RenderHandle.h"
#include "Render/RenderQueue.h"
#include "Core/EventManager.h"
#include "Platform/InputEvents.h"
#include "Platform/WindowEvents.h"
#include "UserInterface/GuiConfig.h"

namespace Tange
{
    // A base GUI "object" is simply a rectangle that can be displayed
    // at a particular window coordinate position.
    class GuiObject
    {
    protected:
        RenderHandle m_hRender = {};
        Transform m_transform = {};
        Vec2 m_position = Vec2(0, 0);
        Vec2 m_scale = Vec2(1, 1);
        bool m_visible = false;

    public:
        GuiObject() = default;

        GuiObject(Vec2 centerP, Vec2 dimensions)
            : m_position(centerP), m_scale(dimensions)
        {
            m_hRender.AttachMesh("DefaultQuad");
            m_hRender.SetColor(Vec4(0, 0, 0, 1));
            m_transform.WindowOrthographic();

            // TODO: Get a proper id from event manager!
            EventManager::BindHandler<WindowResized>(1,
            [this](const IEvent& event)
            {
                const auto& resizeEvent = static_cast<const WindowResized&>(event);

                auto tScale = Vec2(m_scale.Width / resizeEvent.CurrentWidth,
                                   m_scale.Height / resizeEvent.CurrentHeight);
                m_scale = Vec2(resizeEvent.DesiredWidth * tScale.Width,
                               resizeEvent.DesiredHeight * tScale.Height);
                
                auto tPosition = Vec2(m_position.Width / resizeEvent.CurrentWidth,
                                      m_position.Height / resizeEvent.CurrentHeight);
                m_position = Vec2(resizeEvent.DesiredWidth * tPosition.Width,
                                  resizeEvent.DesiredHeight * tPosition.Height);
            });
        }

        GuiObject(Vec2 centerP, Vec2 dimensions, Vec4 color)
            : GuiObject(centerP, dimensions)
        {
            SetColor(color);
        }

        virtual void Destroy()
        {
            EventManager::DetachHandler<WindowResized>(1);
        }

        // By default, all objects simply update with their bounding
        // rectangle. For unordinary cases, this method can be overriden.
        virtual void Update(float timeStep)
        {
            m_transform.Update(Vec3(m_position, 1), 
                               Vec3(m_scale, 1), 
                               Vec3(0, 0 , 0));
        }

        // By default, all objects simply render a filled quad.
        // For unordinary cases, this method can be overriden.
        virtual void Render(const RenderQueue& queue)
        {
            if (m_visible)
            {
                queue.Submit("PixelFill", m_hRender, m_transform);
            }
        }

        virtual void Show()
        {
            m_visible = true;
        }

        virtual void Hide()
        {
            m_visible = false;
        }
        
        void ToggleVisibility()
        {
            // This is done this way as Show/Hide can be overriden.
            if (!m_visible)
            {
                Show();
            }
            else
            {
                Hide();
            }
        }

        void SetColor(Vec4 color)
        {
            m_hRender.SetColor(color);
        }
    };
}