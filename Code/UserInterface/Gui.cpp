#include "UserInterface/Gui.h"

namespace Tange
{
    // TODO: For clickable gui elements, focus recognition for elements is
    // needed. Lets say there was a button overlapped on a panel, the
    // button and panel would both fire off their handler if the button
    // was clicked.
    
    GuiObject::~GuiObject()
    {
        auto &eventManager = EventManager::Get();

        for (auto entity : m_entities)
        {
            eventManager.DetachHandler<MouseClicked>(entity.Id);
        }
    }

    void GuiObject::AddPanel(Vec2 position, Vec2 scale, bool selectable, 
                             bool dragable, bool snapping)
    {
        auto& entityManager = EntityManager::Get();
        auto& resourceManager = ResourceManager::Get();
        auto& eventManager = EventManager::Get();

        auto entity = entityManager.RegisterEntity();
        m_entities.push_back(entity);

        auto& drawable = entityManager.AttachComponent<Drawable>(entity);
        drawable.AttachMesh("DefaultQuad");

        auto& transform = entityManager.AttachComponent<Transformable>(entity);
        transform.SetOrthographic(Vec2(), GetDrawRegion(), 0.1, 100.0);
        transform.Position = position;
        transform.Scale = Vec3(scale.X, scale.Y, 0);

        auto& guiElement = entityManager.AttachComponent<GuiComponent>(entity);
        guiElement.ComputeBoundingBox(position, scale);
        guiElement.Selected = false;
        guiElement.Dragable = dragable;
        guiElement.ShouldSnap = snapping;

        eventManager.BindHandler<MouseClicked>(entity.Id,
        [entity](const IEvent &event) 
        {
            const auto& mouseEvent = static_cast<const MouseClicked&>(event);

            // Selection check on left click.
            if (mouseEvent.Button == InputEvent::LeftClick)
            {
                auto& entityManager = EntityManager::Get();
                auto& guiElement = entityManager.GetComponent<GuiComponent>(entity);

                if (guiElement.CollidesWith(mouseEvent.MousePosition))
                {
                    guiElement.Selected = true;
                    guiElement.PreviousMousePosition = mouseEvent.MousePosition;
                }
            }
        });

        eventManager.BindHandler<MouseReleased>(entity.Id,
        [entity](const IEvent &event) 
        {
            const auto& mouseEvent = static_cast<const MouseReleased&>(event);

            if (mouseEvent.Button == InputEvent::LeftClick)
            {
                auto& entityManager = EntityManager::Get();
                auto& guiElement = entityManager.GetComponent<GuiComponent>(entity);
                auto& transform = entityManager.GetComponent<Transformable>(entity);

                guiElement.Selected = false;
                guiElement.ComputeBoundingBox(transform.Position, Vec2(transform.Scale.X, transform.Scale.Y));
            }
        });

        eventManager.BindHandler<MouseMoved>(entity.Id,
        [entity](const IEvent &event)
        {
            const auto& mouseEvent = static_cast<const MouseMoved&>(event);
            auto& entityManager = EntityManager::Get();
            auto& guiElement = entityManager.GetComponent<GuiComponent>(entity);
            auto& transform = entityManager.GetComponent<Transformable>(entity);
            
            if (guiElement.Selected && guiElement.Dragable)
            {
                auto& platform = PlatformManager::Get();
                
                Vec2i mousePosition = platform.CalculateMousePosition();
                
                if (guiElement.ShouldSnap)
                {
                    // Snap the center of the GUI element to the mouse.
                    transform.Position = Vec2(mousePosition.X, mousePosition.Y);
                }
                else
                {
                    // This moves the GUI element centered around where it was clicked.
                    Vec2i dPosition = mousePosition - guiElement.PreviousMousePosition;
                    guiElement.PreviousMousePosition = mousePosition;
                    transform.Position += dPosition;
                }
            }
        });

        eventManager.BindHandler<UpdateEvent>(entity.Id,
        [entity](const IEvent& event)
        {
            auto& entityManager = EntityManager::Get();
            auto& guiElement = entityManager.GetComponent<GuiComponent>(entity);
            auto& transform = entityManager.GetComponent<Transformable>(entity);
            transform.OnUpdate();
        });

        eventManager.BindHandler<RenderEvent>(entity.Id,
        [entity](const IEvent& event)
        {
            auto& entityManager = EntityManager::Get();
            const auto& drawable = entityManager.GetComponent<Drawable>(entity);
            drawable.OnRender();
        });
    }
}