#include "UserInterface/GuiButton.h"

namespace Tange
{
    GuiButton::GuiButton(const GuiConfig& config, const std::string& label, Vec2 centerP, 
                         Vec2 dimensions, const std::function<void()>& callback)
        : GuiObject(centerP, dimensions), 
          OnClick(callback), m_pOutline(nullptr),
          m_label(label, *config.pFontAtlas, centerP, config.FontPixelHeight, config.FontColor)
    {
    }

    GuiButton::GuiButton(const std::string& label, const FontAtlas& atlas, 
                         Vec2 centerP, Vec2 dimensions,
                         float fontPixelHeight, Vec4 fontColor,
                         const std::function<void()>& callback)
        : GuiObject(centerP, dimensions), 
          OnClick(callback), m_pOutline(nullptr),
          m_label(label, atlas, centerP, fontPixelHeight, fontColor)
    {
    }

    void GuiButton::Destroy()
    {
        GuiObject::Destroy();
        m_label.Destroy();
        
        if (m_pOutline) delete m_pOutline;
    }

    void GuiButton::AddOutline(float thickness, Vec4 color)
    {
        if (m_pOutline) delete m_pOutline;
        m_pOutline = new GuiOutline(m_position, m_scale, thickness, color);
    }

    void GuiButton::AddOutline(const GuiConfig& config)
    {
        if (m_pOutline) delete m_pOutline;        
        m_pOutline = new GuiOutline(config, m_position, m_scale);
    }

    void GuiButton::Update(float timeStep)
    {
        GuiObject::Update(timeStep);
        m_label.Update(timeStep);
        if (m_pOutline) m_pOutline->Update(timeStep);
    }

    void GuiButton::Render(const RenderQueue& queue)
    {
        GuiObject::Render(queue);
        m_label.Render(queue);
        if (m_pOutline) m_pOutline->Render(queue);
    }

    void GuiButton::Show()
    {
        GuiObject::Show();
        m_label.Show();
        if (m_pOutline) m_pOutline->Show();

        // TODO: Get a proper id.
        EventManager::BindHandler<MouseReleased>(1,
        [this](const IEvent& event)
        {
            const auto& mouseEvent = static_cast<const MouseReleased&>(event);

            // Selection check on left click.
            if (mouseEvent.Button == InputEvent::LeftClick)
            {
                // TODO: Do a collision check.
                //if (CollidesWith(mouseEvent.MousePosition))
                {
                    OnClick();
                }
            }
        });
    }

    void GuiButton::Hide()
    {
        GuiObject::Hide();
        m_label.Hide();
        if (m_pOutline) m_pOutline->Hide();

        // TODO: Get a proper id.
        EventManager::DetachHandler<MouseReleased>(1);
    }
}