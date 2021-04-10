#pragma once

#include "Core/Common.h"
#include "UserInterface/GuiObject.h"
#include "UserInterface/GuiConfig.h"
#include "Asset/FontAtlas.h"
#include "Render/RenderQueue.h"

namespace Tange
{
    class GuiMenu
    {
        const RenderQueue& m_renderQueue;
        
        std::vector<std::unique_ptr<GuiObject>> m_objects;

    public:
        explicit GuiMenu(const RenderQueue& queue);
        ~GuiMenu();

        template<typename T>
        void Add(const T& object);

        void Update(float timeStep);
        void Render();

        void ToggleVisibility();
        void Show();
        void Hide();

    private:
        GuiMenu(const GuiMenu&) = delete;
        void operator=(const GuiMenu&) = delete;
    };
}