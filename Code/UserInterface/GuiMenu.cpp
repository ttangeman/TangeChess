#include "UserInterface/GuiMenu.h"

namespace Tange
{
    GuiMenu::GuiMenu(const RenderQueue& queue)
        : m_renderQueue(queue)
    {
    }

    template<typename T>
    void GuiMenu::Add(T&& object)
    {
        m_objects.emplace_back(std::make_unique<T>(object));
    }

    template<typename T>
    void GuiMenu::Add(const T& object)
    {
        m_objects.emplace_back(std::make_unique<T>(object));
    }

    void GuiMenu::Update(float timeStep)
    {
        for (const auto& object : m_objects)
        {
            object->Update(timeStep);
        }
    }

    void GuiMenu::Render()
    {
        for (const auto& object : m_objects)
        {
            object->Render(m_renderQueue);
        }
    }

    void GuiMenu::ToggleVisibility()
    {
        for (const auto& object : m_objects)
        {
            object->ToggleVisibility();
        }
    }

    void GuiMenu::Show()
    {
        for (const auto& object : m_objects)
        {
            object->Show();
        }
    }

    void GuiMenu::Hide()
    {
        for (const auto& object : m_objects)
        {
            object->Hide();
        }
    }
}