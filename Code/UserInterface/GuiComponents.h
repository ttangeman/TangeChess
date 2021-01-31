#pragma once

#include "Core/Common.h"
#include "Entity/Component.h"
#include "Entity/Entity.h"

namespace Tange
{
    class MouseInteractable : public Component<MouseInteractable>
    {
    public:
        Rect BoundingBox;
        Vec2i PreviousMousePosition;

        bool CollidesWith(Vec2i point)
        {
            if ((point.X < BoundingBox.MaxCorner.X && 
                point.Y < BoundingBox.MaxCorner.Y) &&
                (point.X > BoundingBox.MinCorner.X &&
                point.Y > BoundingBox.MinCorner.Y))
            {
                return true;
            } 
            return false;
        }

        void ComputeBoundingBox(Vec2 position, Vec2 scale)
        {
            // The scale _is_ the diameter.
            Vec2 radius = Vec2(scale.X / 2.0, scale.Y / 2.0);
            BoundingBox = Rect(position - radius, position + radius);
        }

        void BindHandlers(bool dragable)
        {
            if (dragable)
            {
                BindDragableMouseClicked();
                BindDefaultMouseReleased();
            }
        }

        void BindDragableMouseClicked()
        {
            auto& eventManager = EventManager::Get();

            eventManager.BindHandler<MouseClicked>(Entity.Id,
            [this](const IEvent& event)
            {
                const auto& mouseEvent = static_cast<const MouseClicked&>(event);

                // Selection check on left click.
                if (mouseEvent.Button == InputEvent::LeftClick)
                {
                    // If a selection is detected, then the MouseMoved handler will
                    // be bound to the entity.
                    if (CollidesWith(mouseEvent.MousePosition))
                    {
                        PreviousMousePosition = mouseEvent.MousePosition;
                        BindMouseMoved();
                    }
                }
            });
        }

        void BindDefaultMouseReleased()
        {
            auto& eventManager = EventManager::Get();

            eventManager.BindHandler<MouseReleased>(Entity.Id,
            [this](const IEvent &event) 
            {
                const auto& mouseEvent = static_cast<const MouseReleased&>(event);

                if (mouseEvent.Button == InputEvent::LeftClick)
                {
                    auto& entityManager = EntityManager::Get();
                    auto& transform = entityManager.GetComponent<Transformable>(Entity);

                    // Only need to update the bounding box when the panel is let go.
                    ComputeBoundingBox(transform.Position, Vec2(transform.Scale.X, transform.Scale.Y));

                    auto& eventManager = EventManager::Get();
                    eventManager.DetachHandler<MouseMoved>(Entity.Id);
                }
            });
        }

        void BindMouseMoved()
        {
            auto& eventManager = EventManager::Get();

            eventManager.BindHandler<MouseMoved>(Entity.Id,
            [this](const IEvent &event)
            {
                const auto& mouseEvent = static_cast<const MouseMoved&>(event);

                auto& entityManager = EntityManager::Get();
                auto& transform = entityManager.GetComponent<Transformable>(Entity);
                
                auto& platform = PlatformManager::Get();
                Vec2i mousePosition = platform.CalculateMousePosition();

                // This snaps the center of the GUI element to the mouse.
                //transform.Position = Vec2(mousePosition.X, mousePosition.Y);

                // This moves the GUI element centered around where it was clicked.
                Vec2i dPosition = mousePosition - PreviousMousePosition;
                PreviousMousePosition = mousePosition;
                transform.Position += dPosition;
            });
        }
    };
}