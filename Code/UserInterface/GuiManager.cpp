#include "UserInterface/GuiManager.h"

namespace Tange
{
    GuiManager GuiManager::s_instance;

    GuiManager::GuiManager()
    {
        m_pAtlas = nullptr;
        m_pRenderQueue = nullptr;
    }

    void GuiManager::SetFontAtlas(FontAtlas& atlas)
    {
        s_instance.m_pAtlas = &atlas;
    }

    const FontAtlas& GuiManager::GetFontAtlas()
    {
        return *s_instance.m_pAtlas;
    }

    void GuiManager::SetRenderQueue(RenderQueue& queue)
    {
        s_instance.m_pRenderQueue = &queue;
    }

    const RenderQueue& GuiManager::GetRenderQueue()
    {
        return *s_instance.m_pRenderQueue;
    }
}