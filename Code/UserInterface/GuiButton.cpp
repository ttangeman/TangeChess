#include "UserInterface/GuiButton.h"

namespace Tange
{
    GuiButton::GuiButton(const std::string& label, float fontPixelSize, Vec4 fontColor, 
                         const std::function<void()>& callback)
        : OnClick(callback)
    {
        m_label.SetText(GuiManager::GetFontAtlas(), label, 
                        m_position, fontPixelSize, fontColor); 
    }
    
    void GuiButton::Render(const RenderQueue& queue)
    {
        GuiObject::Render(queue);
        m_label.Render(queue);
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