#pragma once

#include "Core/Common.h"
#include "UserInterface/Widgets.h"

namespace Tange
{
    class Menu
    {
        std::vector<std::unique_ptr<IWidget>> m_widgets;
        bool m_visible = false;

    public:
        template<typename T>
        void AddWidget(T&& object);

        // Show/hide the menu.
        void Show();
        void Hide();
        void ToggleVisibility();
    };
}