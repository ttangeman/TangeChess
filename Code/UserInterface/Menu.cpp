#include "UserInterface/Menu.h"

namespace Tange
{
    void Menu::AddPanel(Vec2 position, Vec2 scale, Vec4 color)
    {
        auto entity = EntityManager::RegisterEntity();

        auto& drawable = EntityManager::AttachComponent<Drawable>(entity);
        drawable.AttachMesh("DefaultQuad");
        drawable.SetColor(color);

        auto& transform = EntityManager::AttachComponent<Transformable>(entity);
        transform.SetOrthographic(Vec2(), GetDrawRegion(), 0.1, 100.0);
        transform.Position = position;
        transform.Scale = Vec3(scale.X, scale.Y, 0);

        auto& dragable = EntityManager::AttachComponent<Dragable2D>(entity);
        dragable.ComputeBoundingBox(transform.Position, transform.Scale);

        m_entities.emplace_back(entity);
    }

    void Menu::AddButton(Vec2 position, Vec2 scale, Vec4 color, 
                         std::function<void()> callback)
    {
        auto entity = EntityManager::RegisterEntity();

        auto& drawable = EntityManager::AttachComponent<Drawable>(entity);
        drawable.AttachMesh("DefaultQuad");
        drawable.SetColor(color);

        auto& transform = EntityManager::AttachComponent<Transformable>(entity);
        transform.SetOrthographic(Vec2(), GetDrawRegion(), 0.1, 100.0);
        transform.Position = position;
        transform.Scale = Vec3(scale.X, scale.Y, 0);

        auto& clickable = EntityManager::AttachComponent<Clickable2D>(entity);
        clickable.ComputeBoundingBox(transform.Position, transform.Scale);
        clickable.OnClick = callback;

        m_entities.emplace_back(entity);
    }

    void Menu::OnUpdate()
    {
        if (m_visible)
        {
            for (auto entity : m_entities)
            {
                auto& transform = EntityManager::AttachComponent<Transformable>(entity);
                transform.OnUpdate();
            }
        }
    }

    void Menu::OnRender()
    {
        if (m_visible)
        {
            for (auto entity : m_entities)
            {
                auto& transform = EntityManager::AttachComponent<Transformable>(entity);
                auto& drawable = EntityManager::AttachComponent<Drawable>(entity);

                transform.OnRender();
                drawable.OnRender();
            }
        }
    }

    void Menu::Show()
    {
        m_visible = true;

        for (auto entity : m_entities)
        {
            if (EntityManager::HasComponent<Dragable2D>(entity))
            {
                auto& dragable = EntityManager::GetComponent<Dragable2D>(entity);
                dragable.BindInputHandlers();
            }

            if (EntityManager::HasComponent<Clickable2D>(entity))
            {
                auto& clickable = EntityManager::GetComponent<Clickable2D>(entity);
                clickable.BindInputHandlers();
            }
        }
    }

    void Menu::Hide()
    {
        m_visible = false;

        for (auto& entity : m_entities)
        {
            EventManager::DetachAllHandlers(entity.Id);
        }
    }

    void Menu::ToggleVisibility()
    {
        if (!m_visible)
        {
            Show();
        }
        else
        {
            Hide();
        }
    }
}