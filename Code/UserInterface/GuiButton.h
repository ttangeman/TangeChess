#pragma once

#include "UserInterface/GuiObject.h"
#include "UserInterface/TextLine.h"
#include "UserInterface/GuiManager.h"

namespace Tange
{
    // A button is a base GUI object that can be clicked
    // and has a text label.
    class GuiButton : public GuiObject
    {
        TextLine m_label;

    public:
        std::function<void()> OnClick;

        GuiButton(const std::string& label, float fontPixelSize, Vec4 fontColor, 
                  const std::function<void()>& callback);

        void Render(const RenderQueue& queue) override;
        void Show() override;
        void Hide() override;
    };
}