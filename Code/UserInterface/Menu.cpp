#include "UserInterface/Menu.h"

namespace Tange
{
    template<typename T>
    void Menu::AddWidget(T&& object)
    {
        m_widgets.push_back(std::make_unique<T>(std::move(object)));
    }

    void Menu::Show()
    {
        for (auto& it : m_widgets)
        {
            it->Show();
        }
    }

    void Menu::Hide()
    {
        for (auto& it : m_widgets)
        {
            it->Hide();
        }
    }

    void Menu::ToggleVisibility()
    {
        if (m_visible)
        {
            Hide();
        }
        else
        {
            Show();
        }
        m_visible = !m_visible;
    }
}