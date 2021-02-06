#pragma once

#include "Core/Common.h"
#include "Entity/Component.h"
#include "Entity/Entity.h"
#include "Render/ResourceManager.h"

namespace Tange
{
    class Text : public Component<Text>
    {
        Entity m_textEntity;
        std::string m_text;

    public:
        void CreateText(const FontAtlas& atlas, const std::string& text, 
                         Vec2 position, Vec4 color, float pixelHeight)
        {
            m_text = text;
            m_textEntity = EntityManager::RegisterEntity();

            float scale = pixelHeight / atlas.GlyphPixelSize;
            float textLineWidth = 0;

            // Loop once over the text to compute a bounding rectangle
            // for the line(s) of text.
            for (auto i = 0; i < text.length(); i++)
            {
                const auto& glyphInfo = atlas.LookupGlyphInfo(text[i]);

                textLineWidth += glyphInfo.AdvanceX * scale;
            }

            // TODO: Text wrapping if it goes offscreen!
            ASSERT(textLineWidth <= GetDrawRegion().Width);

            // Center the bounding box at the specified position.
            auto adjustedP = Vec2(position.X - (textLineWidth / 2.0),
                                  position.Y - (pixelHeight / 2.0));

            Quad* pQuads = (Quad*)malloc(sizeof(Quad) * text.length());

            // Create a batched quad for all of the glyphs.
            for (auto i = 0; i < text.length(); i++)
            {
                const auto& glyphInfo = atlas.LookupGlyphInfo(text[i]);

                // NOTE: Y is offset by the glyph height to account for the bitmap being flipped.
                Vec2 minP = Vec2(adjustedP.X + glyphInfo.OffsetX * scale, 
                                 adjustedP.Y + (glyphInfo.OffsetY - glyphInfo.Size.Height) * scale);
                Vec2 maxP = Vec2(minP.X + glyphInfo.Size.Width * scale,
                                 minP.Y + glyphInfo.Size.Height * scale);

                pQuads[i] = Quad::CreatePreTransformed(minP, maxP, color, 
                                                       glyphInfo.MinTexCoords, 
                                                       glyphInfo.MaxTexCoords);

                adjustedP.X += glyphInfo.AdvanceX * scale;
            }

            ResourceManager::SubmitMesh(text, pQuads, 
                                        Quad::VerticeCount * text.length(), 
                                        sizeof(Vertex));

            auto& drawable = EntityManager::AttachComponent<Drawable>(m_textEntity);
            drawable.AttachMesh(text);
            drawable.AttachTexture(atlas.FontName);

            auto& transform = EntityManager::AttachComponent<Transformable>(m_textEntity);
            transform.SetOrthographic(Vec2(), GetDrawRegion(), 0.1, 100.0);

            free(pQuads);
        }

        // NOTE: Must be called explicitly.
        void Destroy()
        {
            if (!m_text.empty())
            {
                ResourceManager::ReleaseMesh(m_text);
            }

            if (m_textEntity.IsValid())
            {
                EntityManager::DestroyEntity(m_textEntity);
            }
        }

        void OnRender()
        {
            auto& drawable = EntityManager::GetComponent<Drawable>(m_textEntity);
            auto& transform = EntityManager::GetComponent<Transformable>(m_textEntity);

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