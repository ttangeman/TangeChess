#pragma once

#include "UserInterface/GuiObject.h"
#include "UserInterface/TextLine.h"
#include "UserInterface/GuiOutline.h"

namespace Tange
{
    // A button is a base GUI object that can be clicked
    // and has a text label.
    class GuiButton : public GuiObject
    {
        TextLine m_label;
        GuiOutline* m_pOutline;

    public:
        std::function<void()> OnClick;

        GuiButton(const GuiConfig& config, const std::string& label, 
                  Vec2 centerP, Vec2 dimensions, 
                  const std::function<void()>& callback);

        GuiButton(const std::string& label, const FontAtlas& atlas, 
                  Vec2 centerP, Vec2 dimensions,
                  float fontPixelHeight, Vec4 fontColor,
                  const std::function<void()>& callback);

        void Destroy() override;

        void AddOutline(float thickness = 1.0, Vec4 color = Vec4(1, 1, 1, 1));
        void AddOutline(const GuiConfig& config);

        void Update(float timeStep) override;                  
        void Render(const RenderQueue& queue) override;
        void Show() override;
        void Hide() override;
    };
}