#pragma once

#include "Core/Common.h"
#include "Entity/Component.h"
#include "Entity/Entity.h"
#include "Render/ResourceManager.h"

namespace Tange
{
    struct Label : public Component<Label>
    {
        Entity TextEntity;
        std::string Text;

        void CreateLabel(const FontAtlas& atlas, const std::string& text, 
                         Vec2 position, Vec4 color)
        {
            Text = text;
            TextEntity = EntityManager::RegisterEntity();

            Quad* quads = (Quad*)malloc(sizeof(Quad) * text.length());

            float scale = 0.5;

            for (auto i = 0; i < text.length(); i++)
            {
                const auto& glyphInfo = atlas.LookupGlyphInfo(text[i]);

                // NOTE: Y is offset by the glyph height to account for the bitmap being flipped.
                Vec2 minPosition = Vec2(position.X + glyphInfo.OffsetX * scale, 
                                        position.Y + (glyphInfo.OffsetY - glyphInfo.GlyphSize.Height) * scale);
                Vec2 maxPosition = Vec2(minPosition.X + glyphInfo.GlyphSize.Width * scale,
                                        minPosition.Y + glyphInfo.GlyphSize.Height * scale);

                quads[i] = Quad::CreatePreTransformed(minPosition, maxPosition, color, 
                                                      glyphInfo.MinTexCoords, 
                                                      glyphInfo.MaxTexCoords);

                position.X += glyphInfo.AdvanceX * scale;
            }

            ResourceManager::SubmitMesh(text, quads, 
                                        Quad::VerticeCount * text.length(), 
                                        sizeof(Vertex));

            auto& drawable = EntityManager::AttachComponent<Drawable>(TextEntity);
            drawable.AttachMesh(text);
            drawable.AttachTexture(atlas.FontName);
            drawable.SetColor(Vec4(1, 1, 1, 1));

            auto& transform = EntityManager::AttachComponent<Transformable>(TextEntity);
            transform.Scale = Vec3(1, 1, 1);
            transform.SetOrthographic(Vec2(), GetDrawRegion(), 0.1, 100.0);

            free(quads);
        }

        // NOTE: Must be called explicitly.
        void Destroy()
        {
            if (!Text.empty())
            {
                ResourceManager::ReleaseMesh(Text);
            }

            if (TextEntity.IsValid())
            {
                EntityManager::DestroyEntity(TextEntity);
            }
        }

        void OnRender()
        {
            auto& drawable = EntityManager::GetComponent<Drawable>(TextEntity);
            auto& transform = EntityManager::GetComponent<Transformable>(TextEntity);

            transform.OnUpdate();
            transform.OnRender();
            drawable.OnRender();
        }
    };

    struct Outline2D : public Component<Outline2D>
    {
        float Thickness;
    };

    struct Dragable2D : public Component<Dragable2D>
    {
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

        void ComputeBoundingBox(Vec2 position, Vec3 scale)
        {
            // The scale _is_ the diameter.
            Vec2 radius = Vec2(scale.X / 2.0, scale.Y / 2.0);
            BoundingBox = Rect(position - radius, position + radius);
        }
        
        void BindMouseMoved()
        {
            EventManager::BindHandler<MouseMoved>(BoundEntity.Id,
            [this](const IEvent &event)
            {
                const auto& mouseEvent = static_cast<const MouseMoved&>(event);

                auto& transform = EntityManager::GetComponent<Transformable>(BoundEntity);
                
                Vec2i mousePosition = PlatformManager::CalculateMousePosition();

                // This snaps the center of the GUI element to the mouse.
                //transform.Position = Vec2(mousePosition.X, mousePosition.Y);

                // This moves the GUI element centered around where it was clicked.
                Vec2i dPosition = mousePosition - PreviousMousePosition;
                PreviousMousePosition = mousePosition;
                transform.Position += dPosition;
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
                    // If a selection is detected, then the MouseMoved handler will
                    // be bound to the entity.
                    if (CollidesWith(mouseEvent.MousePosition))
                    {
                        PreviousMousePosition = mouseEvent.MousePosition;
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
                    auto& transform = EntityManager::GetComponent<Transformable>(BoundEntity);

                    // Only need to update the bounding box when the panel is let go.
                    ComputeBoundingBox(transform.Position, transform.Scale);
                    
                    EventManager::DetachHandler<MouseMoved>(BoundEntity.Id);
                }
            });
        }
    };

    struct Clickable2D : Component<Clickable2D>
    {
        Rect BoundingBox;
        std::function<void()> OnClick;

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
        
        void ComputeBoundingBox(Vec2 position, Vec3 scale)
        {
            // The scale _is_ the diameter.
            Vec2 radius = Vec2(scale.X / 2.0, scale.Y / 2.0);
            BoundingBox = Rect(position - radius, position + radius);
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
    };
}