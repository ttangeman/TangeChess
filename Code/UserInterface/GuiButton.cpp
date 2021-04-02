#include "UserInterface/GuiButton.h"

namespace Tange
{
    GuiButton::GuiButton(Vec2 centerP, Vec2 dimensions, const FontAtlas& atlas, 
                         const std::string& label, float fontPixelSize, Vec4 fontColor, 
                         const std::function<void()>& callback)
        : GuiObject(centerP, dimensions), OnClick(callback)
    {
        m_label.SetText(atlas, label, m_position, fontPixelSize, fontColor); 
    }

    GuiButton::GuiButton(Vec2 centerP, Vec2 dimensions, Vec4 color, 
                         const FontAtlas& atlas, const std::string& label, 
                         float fontPixelSize, Vec4 fontColor, 
                         const std::function<void()>& callback)
        : GuiObject(centerP, dimensions, color), OnClick(callback)
    {
        m_label.SetText(atlas, label, m_position, fontPixelSize, fontColor); 
    }

    void GuiButton::Render(const RenderQueue& queue)
    {
        GuiObject::Render(queue);

        if (m_visible)
        {
            m_label.Render(queue);
        }
    }

    void GuiButton::Show()
    {
        GuiObject::Show();

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

        // TODO: Get a proper id.
        EventManager::DetachHandler<MouseReleased>(1);
    }
}