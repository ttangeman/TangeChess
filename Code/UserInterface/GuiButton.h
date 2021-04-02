#pragma once

#include "UserInterface/GuiObject.h"
#include "UserInterface/TextLine.h"

namespace Tange
{
    // A button is a base GUI object that can be clicked
    // and has a text label.
    class GuiButton : public GuiObject
    {
        TextLine m_label;

    public:
        std::function<void()> OnClick;

        GuiButton(Vec2 centerP, Vec2 dimensions, const FontAtlas& atlas, 
                  const std::string& label, float fontPixelSize, 
                  Vec4 fontColor, const std::function<void()>& callback);
        
        GuiButton(Vec2 centerP, Vec2 dimensions, Vec4 color, 
                  const FontAtlas& atlas, const std::string& label, 
                  float fontPixelSize, Vec4 fontColor, const std::function<void()>& callback);
                  
        void Render(const RenderQueue& queue) override;
        void Show() override;
        void Hide() override;
    };
}