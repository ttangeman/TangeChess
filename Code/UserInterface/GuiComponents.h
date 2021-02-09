#pragma once

#include "Core/Common.h"
#include "Entity/Component.h"
#include "Entity/Entity.h"
#include "Entity/EntityManager.h"
#include "Render/ResourceManager.h"
#include "Platform/PlatformManager.h"

namespace Tange
{
    // Used to tag text so that it can be appropiately destroyed.
    struct TextTag : public Component<TextTag>
    {
        std::string Text;

        void Destroy()
        {
            if (!Text.empty())
            {
                ResourceManager::ReleaseMesh(Text);
            }
            EntityManager::DestroyEntity(BoundEntity);
        }
    };

    struct Outline2D : public Component<Outline2D>
    {
        float Thickness;
    };

    // TODO: Dragable is currently busted because it does not move text/outlines.
    // Probably need some kind of GuiComponent that tracks the parent/children entities.
    // The text entity storing a separate entity from the one it's bound to is quite janky,
    // for example.
    struct Dragable2D : public Component<Dragable2D>
    {
        // The dimensions for the "click box".
        Vec2 ClickPosition;
        Vec2 ClickScale;

        // Tracks previous mouse position.
        Vec2 PreviousMousePosition;

        // Used for mapping the mouse position into a coordinate system
        // that is not window coordinates (the default).
        bool MapMouseCoordinates = false;
        Vec2 DestMouseMin;
        Vec2 DestMouseMax;

        void Initialize(Vec2 position, Vec2 scale)
        {
            ClickPosition = position;
            ClickScale = scale;

            EventManager::BindHandler<WindowResized>(BoundEntity.Id,
            [this](const IEvent& event)
            {
                const auto& resizeEvent = static_cast<const WindowResized&>(event);
                // TODO: Account for mouse mapping?
                auto tScale = Vec2(ClickScale.Width / resizeEvent.CurrentWidth,
                                   ClickScale.Height / resizeEvent.CurrentHeight);
                ClickScale = Vec2(resizeEvent.DesiredWidth * tScale.Width,
                                  resizeEvent.DesiredHeight * tScale.Height);
                
                auto tPosition = Vec2(ClickPosition.Width / resizeEvent.CurrentWidth,
                                      ClickPosition.Height / resizeEvent.CurrentHeight);
                ClickPosition = Vec2(resizeEvent.DesiredWidth * tPosition.Width,
                                     resizeEvent.DesiredHeight * tPosition.Height);
            });
        }

        void SetMouseMapping(Vec2 min, Vec2 max)
        {
            MapMouseCoordinates = true;
            DestMouseMin = min;
            DestMouseMax = max;
        }

        bool CollidesWith(Vec2 point)
        {
            auto boundingBox = Rect::ComputeBoundingBox(ClickPosition, ClickScale);

            if ((point.X < boundingBox.MaxCorner.X && 
                point.Y < boundingBox.MaxCorner.Y) &&
                (point.X > boundingBox.MinCorner.X &&
                point.Y > boundingBox.MinCorner.Y))
            {
                return true;
            } 
            return false;
        }
        
        void BindMouseMoved()
        {
            EventManager::BindHandler<MouseMoved>(BoundEntity.Id,
            [this](const IEvent &event)
            {
                const auto& mouseEvent = static_cast<const MouseMoved&>(event);

                auto& worldP = EntityManager::GetComponent<WorldTransform>(BoundEntity);
                
                Vec2 mouseP = PlatformManager::CalculateMousePosition();

                if (MapMouseCoordinates)
                {
                    // Normally mouse coordinates are in window space.
                    Vec2 renderDim = GetDrawRegion();
                    Vec2 tPosition = Vec2(mouseP.X / renderDim.Width,
                                          mouseP.Y / renderDim.Height);
                    mouseP.X = Lerp(DestMouseMin.X, tPosition.X, DestMouseMax.X);
                    mouseP.Y = Lerp(DestMouseMin.Y, tPosition.Y, DestMouseMax.Y);
                }
#if 1
                // This snaps the center of the GUI element to the mouse.
                worldP.Position.XY = mouseP;
#else
                // This moves the GUI element centered around where it was clicked.
                Vec2i dPosition = mouseP - PreviousMousePosition;
                worldP.Position += dPosition;
#endif
                PreviousMousePosition = mouseP;
                // Update the dragables position.
                ClickPosition = worldP.Position.XY; 
            });
        }
        
        void BindInputHandlers()
        {
            EventManager::BindHandler<MouseClicked>(BoundEntity.Id,
            [this](const IEvent& event)
            {
                const auto& mouseEvent = static_cast<const MouseClicked&>(event);

                // Selection check on left click.
                if (mouseEvent.Button == InputEvent::LeftClick)
                {
                    Vec2 mouseP = mouseEvent.MousePosition;

                    // Map mouse coordinates to the desired space, if requested.
                    if (MapMouseCoordinates)
                    {
                        Vec2 renderDim = GetDrawRegion();
                        Vec2 tPosition = Vec2(mouseP.X / renderDim.Width,
                                              mouseP.Y / renderDim.Height);
                        mouseP.X = Lerp(DestMouseMin.X, tPosition.X, DestMouseMax.X);
                        mouseP.Y = Lerp(DestMouseMin.Y, tPosition.Y, DestMouseMax.Y);
                    }

                    // If a selection is detected, then the MouseMoved handler will
                    // be bound to the entity.
                    if (CollidesWith(mouseP))
                    {
                        PreviousMousePosition = mouseP;
                        BindMouseMoved();
                    }
                }
            });

            EventManager::BindHandler<MouseReleased>(BoundEntity.Id,
            [this](const IEvent &event) 
            {
                const auto& mouseEvent = static_cast<const MouseReleased&>(event);

                if (mouseEvent.Button == InputEvent::LeftClick)
                {
                    EventManager::DetachHandler<MouseMoved>(BoundEntity.Id);
                }
            });
        }

        void DetachInputHandlers()
        {
            EventManager::DetachHandler<MouseClicked>(BoundEntity.Id);
            EventManager::DetachHandler<MouseReleased>(BoundEntity.Id);
            EventManager::DetachHandler<MouseMoved>(BoundEntity.Id);
        }
    };

    struct Clickable2D : Component<Clickable2D>
    {
        // The dimensions for the "click box".
        Vec2 ClickPosition;
        Vec2 ClickScale;

        std::function<void()> OnClick;

        void Initialize(Vec2 position, Vec2 scale, 
                        const std::function<void()>& onClick)
        {
            ClickPosition = position;
            ClickScale = scale;
            OnClick = onClick;

            EventManager::BindHandler<WindowResized>(BoundEntity.Id,
            [this](const IEvent& event)
            {
                const auto& resizeEvent = static_cast<const WindowResized&>(event);

                auto tScale = Vec2(ClickScale.Width / resizeEvent.CurrentWidth,
                                   ClickScale.Height / resizeEvent.CurrentHeight);
                ClickScale = Vec2(resizeEvent.DesiredWidth * tScale.Width,
                                  resizeEvent.DesiredHeight * tScale.Height);
                
                auto tPosition = Vec2(ClickPosition.Width / resizeEvent.CurrentWidth,
                                      ClickPosition.Height / resizeEvent.CurrentHeight);
                ClickPosition = Vec2(resizeEvent.DesiredWidth * tPosition.Width,
                                     resizeEvent.DesiredHeight * tPosition.Height);
            });
        }

        bool CollidesWith(Vec2 point)
        {
            auto boundingBox = Rect::ComputeBoundingBox(ClickPosition, ClickScale);

            if ((point.X < boundingBox.MaxCorner.X && 
                point.Y < boundingBox.MaxCorner.Y) &&
                (point.X > boundingBox.MinCorner.X &&
                point.Y > boundingBox.MinCorner.Y))
            {
                return true;
            } 
            return false;
        }

        void BindInputHandlers()
        {
            EventManager::BindHandler<MouseReleased>(BoundEntity.Id,
            [this](const IEvent& event)
            {
                const auto& mouseEvent = static_cast<const MouseReleased&>(event);

                // Selection check on left click.
                if (mouseEvent.Button == InputEvent::LeftClick)
                {
                    // If a selection is detected, then the MouseMoved handler will
                    // be bound to the entity.
                    if (CollidesWith(mouseEvent.MousePosition))
                    {
                        OnClick();
                    }
                }
            });
        }

        void DetachInputHandlers()
        {
            EventManager::DetachHandler<MouseReleased>(BoundEntity.Id);
        }
    };
}