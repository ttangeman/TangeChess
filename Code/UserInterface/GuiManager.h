#pragma once

#include "Core/Common.h"
#include "Asset/FontAtlas.h"
#include "Render/RenderQueue.h"

namespace Tange
{
    class GuiManager
    {
        static GuiManager s_instance;

        RenderQueue* m_pRenderQueue;
        FontAtlas* m_pAtlas;

    public:
        GuiManager();
        GuiManager(const GuiManager&) = delete;
        void operator=(const GuiManager&) = delete;

        static void SetFontAtlas(FontAtlas& atlas);
        static const FontAtlas& GetFontAtlas();

        static void SetRenderQueue(RenderQueue& queue);
        static const RenderQueue& GetRenderQueue();
    };
}