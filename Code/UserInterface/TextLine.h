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
    // A line of text is a special case of a GuiObject
    // that warrants its own class. This should be treated 
    // as the child of objects, so show/hide methods are not 
    // needed since that will be shared with the parent.
    class TextLine
    {
        std::string text;
        RenderHandle m_hRender = {};
        Transform m_transform = {};

        Vec2 m_position = {};
        Vec2 m_scale = {};

    public:
        TextLine();
        void Destroy();

        void Update();
        void Render(const RenderQueue& queue);

        void SetText(const FontAtlas& atlas, const std::string& text, 
                     Vec2 position, float pixelHeight, Vec4 color);
    };
}