#include "UserInterface/Menu.h"

namespace Tange
{
    void Menu::SetBaseColor(Vec4 color)
    {
        m_baseColor = color;
    }

    void Menu::SetOutlineColor(Vec4 color)
    {
        m_outlineColor = color;
    }

    // TODO: Use the same entity for outlines
    void Menu::PushRect(Vec2 position, Vec2 scale, bool outline)
    {
        auto entity = EntityManager::RegisterEntity();

        auto& drawable = EntityManager::AttachComponent<Drawable>(entity);
        drawable.AttachMesh("DefaultQuad");
        drawable.SetColor(m_baseColor);

        auto& transform = EntityManager::AttachComponent<Transformable>(entity);
        transform.SetOrthographic(Vec2(), GetDrawRegion(), 0.1, 100.0);
        transform.Position = position;
        transform.Scale = Vec3(scale.X, scale.Y, 0);

        auto& dragable = EntityManager::AttachComponent<Dragable2D>(entity);
        dragable.ComputeBoundingBox(transform.Position, transform.Scale);

        if (outline)
        {
            auto& outline2d = EntityManager::AttachComponent<Outline2D>(entity);
            outline2d.Thickness = 2.0;
        }

        m_entities.emplace_back(entity);
    }

    void Menu::PushButton(Vec2 position, Vec2 scale, bool outline,
                          std::function<void()> callback) 
    {
        auto entity = EntityManager::RegisterEntity();

        auto& drawable = EntityManager::AttachComponent<Drawable>(entity);
        drawable.AttachMesh("DefaultQuad");
        drawable.SetColor(m_baseColor);

        auto& transform = EntityManager::AttachComponent<Transformable>(entity);
        transform.SetOrthographic(Vec2(), GetDrawRegion(), 0.1, 100.0);
        transform.Position = position;
        transform.Scale = Vec3(scale.X, scale.Y, 0);

        auto& clickable = EntityManager::AttachComponent<Clickable2D>(entity);
        clickable.ComputeBoundingBox(transform.Position, transform.Scale);
        clickable.OnClick = callback;

        if (outline)
        {
            auto& outline2d = EntityManager::AttachComponent<Outline2D>(entity);
            outline2d.Thickness = 2.0;
        }

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
                auto& transform = EntityManager::GetComponent<Transformable>(entity);
                auto& drawable = EntityManager::GetComponent<Drawable>(entity);

                transform.OnRender();
                drawable.OnRender();

                // A tad janky, but the entity is reused to draw outlines.
                // Ideally, a quad batching system would be used for a single mesh,
                // but that is currently not implemented and that has issues of its own
                // (have to modify the mesh instead of using transforms.)
                if (EntityManager::HasComponent<Outline2D>(entity))
                {
                    auto& outline2d = EntityManager::GetComponent<Outline2D>(entity);
                    float thickness = outline2d.Thickness;

                    // Copy the original transform to reinstate its old transform later.
                    auto baseTransform = transform;
                    drawable.SetColor(m_outlineColor);

                    Vec2 radius = Vec2(transform.Scale.X / 2.0, transform.Scale.Y / 2.0);
                
                    // Left rectangle.
                    transform.Position = Vec2(baseTransform.Position.X - radius.X, 
                                              baseTransform.Position.Y);
                    transform.Scale = Vec3(thickness, baseTransform.Scale.Y + thickness, 1);
                    transform.OnUpdate();
                    drawable.OnRender();

                    // Top rectangle.
                    transform.Position = Vec2(baseTransform.Position.X, 
                                              baseTransform.Position.Y + radius.Y);
                    transform.Scale = Vec3(baseTransform.Scale.X + thickness, thickness, 1);
                    transform.OnUpdate();
                    drawable.OnRender();

                    // Right rectangle
                    transform.Position = Vec2(baseTransform.Position.X + radius.X,
                                              baseTransform.Position.Y);
                    transform.Scale = Vec3(thickness, baseTransform.Scale.Y + thickness, 1);
                    transform.OnUpdate();
                    drawable.OnRender();

                    // Bottom rectangle
                    transform.Position = Vec2(baseTransform.Position.X, 
                                              baseTransform.Position.Y - radius.Y);
                    transform.Scale = Vec3(baseTransform.Scale.X + thickness, thickness, 1);
                    transform.OnUpdate();
                    drawable.OnRender();

                    transform.Position = baseTransform.Position;
                    transform.Scale = baseTransform.Scale;
                    drawable.SetColor(m_baseColor);
                }
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