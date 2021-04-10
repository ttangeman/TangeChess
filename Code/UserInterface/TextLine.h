#pragma once

#include "Core/Common.h"
#include "Asset/FontAtlas.h"
#include "Render/Renderer.h"
#include "Render/RenderHandle.h"
#include "Render/RenderQueue.h"
#include "Core/EventManager.h"
#include "Platform/InputEvents.h"
#include "Platform/WindowEvents.h"

namespace Tange
{
    class TextLine : public GuiObject
    {
        std::string m_text;

    public:
        TextLine(const GuiConfig& config, const std::string& text, Vec2 position);
        TextLine(const std::string& text, const FontAtlas& atlas, 
                 Vec2 position, float pixelHeight, Vec4 color);

        void Render(const RenderQueue& queue) override;
    };
}