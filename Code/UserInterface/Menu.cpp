#include "UserInterface/Menu.h"

namespace Tange
{
    Menu::Menu(const RenderQueue& renderQueue, const FontAtlas& atlas)
        : m_renderQueue(renderQueue),
          m_atlas(atlas)
    {
    }

    Menu::~Menu()
    {
        for (auto entity : m_entities)
        {
            if (EntityManager::HasComponent<TextTag>(entity))
            {
                auto& label = EntityManager::GetComponent<TextTag>(entity);
                label.Destroy();
            }
            EntityManager::DestroyEntity(entity);
        }
    }

    void Menu::SetBaseColor(Vec4 color)
    {
        m_baseColor = color;
    }

    void Menu::SetOutlineColor(Vec4 color)
    {
        m_outlineColor = color;
    }

    void Menu::SetTextColor(Vec4 color)
    {
        m_textColor = color;
    }

    void Menu::PushText(const std::string& text, Vec2 position, float pixelHeight)
    {
        if (text.empty()) return;

        auto entity = EntityManager::RegisterEntity();
        entity.Transform.WindowOrthographic();
        
        auto& tag = EntityManager::AttachComponent<TextTag>(entity);
        tag.Text = text;

        float scale = pixelHeight / m_atlas.GlyphPixelSize;
        float textLineWidth = 0;

        // Loop once over the text to compute a bounding rectangle
        // for the line(s) of text.
        for (auto i = 0; i < text.length(); i++)
        {
            const auto& glyphInfo = m_atlas.LookupGlyphInfo(text[i]);
            // NOTE: XAdvance accounts for spaces, glyphInfo.Size does not!
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
            const auto& glyphInfo = m_atlas.LookupGlyphInfo(text[i]);

            // NOTE: Y is offset by the glyph height to account for the bitmap being flipped.
            Vec2 minP = Vec2(adjustedP.X + glyphInfo.OffsetX * scale, 
                             adjustedP.Y + (glyphInfo.OffsetY - glyphInfo.Size.Height) * scale);
            Vec2 maxP = Vec2(minP.X + glyphInfo.Size.Width * scale,
                             minP.Y + glyphInfo.Size.Height * scale);

            pQuads[i] = Quad::CreatePreTransformed(minP, maxP, m_textColor, 
                                                   glyphInfo.MinTexCoords, 
                                                   glyphInfo.MaxTexCoords);

            adjustedP.X += glyphInfo.AdvanceX * scale;
        }

        ResourceManager::SubmitMesh(text, pQuads, 
                                    Quad::VerticeCount * text.length(), 
                                    sizeof(Vertex));

        entity.hRender.AttachMesh(text);
        entity.hRender.AttachTexture(m_atlas.FontName);

        free(pQuads);

        m_entities.push_back(entity);
    }

    void Menu::PushPanel(Vec2 position, Vec2 scale, float outlineThickness)
    {
        auto entity = EntityManager::RegisterEntity();
        entity.hRender.AttachMesh("DefaultQuad");
        entity.hRender.SetColor(m_baseColor);
        entity.Transform.WindowOrthographic();

        auto& worldP = EntityManager::AttachComponent<WorldTransform>(entity);
        worldP.Position.XY = position;
        worldP.Scale.XY = scale;

        // TODO: Need to detect focus for overlapped clickboxes.
        //auto& dragable = EntityManager::AttachComponent<Dragable2D>(entity);
        /*dragable.Initialize(position, scale);*/

        if (outlineThickness > 0.0)
        {
            auto& outline2d = EntityManager::AttachComponent<Outline2D>(entity);
            outline2d.Thickness = outlineThickness;
        }

        m_entities.emplace_back(entity);
    }

    void Menu::PushButton(Vec2 position, Vec2 scale, float outlineThickness,
                          const std::string& text, const std::function<void()>& callback) 
    {
        auto entity = EntityManager::RegisterEntity();
        entity.hRender.AttachMesh("DefaultQuad");
        entity.hRender.SetColor(m_baseColor);
        entity.Transform.WindowOrthographic();

        auto& worldP = EntityManager::AttachComponent<WorldTransform>(entity);
        worldP.Position.XY = position;
        worldP.Scale.XY = scale;

        auto& clickable = EntityManager::AttachComponent<Clickable2D>(entity);
        clickable.Initialize(position, scale, callback);

        if (outlineThickness > 0.0)
        {
            auto& outline2d = EntityManager::AttachComponent<Outline2D>(entity);
            outline2d.Thickness = outlineThickness;
        }

        m_entities.emplace_back(entity);

        if (!text.empty())
        {
            PushText(text, position, scale.Y * 0.25);
        }
    }

    void Menu::Update(float timeStep) 
    {
        if (m_visible)
        {
            for (auto entity : m_entities)
            {   
                if (EntityManager::HasComponent<WorldTransform>(entity))
                {                
                    auto& worldP = EntityManager::GetComponent<WorldTransform>(entity);
                    entity.Transform.Update(worldP.Position, worldP.Scale, worldP.Orientation);
                }
            }
        }
    }

    void Menu::Render()
    {
        if (m_visible)
        {
            for (auto entity : m_entities)
            {
                if (!entity.hRender.hTexture.IsValid())
                {
                    m_renderQueue.Submit("PixelFill", entity.hRender, entity.Transform);

                    if (EntityManager::HasComponent<Outline2D>(entity))
                    {
                        auto& outline2d = EntityManager::GetComponent<Outline2D>(entity);
                        float thickness = outline2d.Thickness;

                        // Copy the original transform to reinstate it later.
                        auto& worldP = EntityManager::GetComponent<WorldTransform>(entity);
                        auto oldWorldP = worldP;
                        entity.hRender.SetColor(m_outlineColor);

                        Vec2 radius = Vec2(worldP.Scale.X / 2.0, worldP.Scale.Y / 2.0);
                    
                        // Left rectangle.
                        worldP.Position.XY = Vec2(oldWorldP.Position.X - radius.X, 
                                            oldWorldP.Position.Y);
                        worldP.Scale = Vec3(thickness, oldWorldP.Scale.Y + thickness, 1);
                        entity.Transform.Update(worldP.Position, worldP.Scale, worldP.Orientation);
                        m_renderQueue.Submit("PixelFill", entity.hRender, entity.Transform);

                        // Top rectangle.
                        worldP.Position.XY = Vec2(oldWorldP.Position.X, 
                                                oldWorldP.Position.Y + radius.Y);
                        worldP.Scale = Vec3(oldWorldP.Scale.X + thickness, thickness, 1);
                        entity.Transform.Update(worldP.Position, worldP.Scale, worldP.Orientation);
                        m_renderQueue.Submit("PixelFill", entity.hRender, entity.Transform);

                        // Right rectangle
                        worldP.Position.XY = Vec2(oldWorldP.Position.X + radius.X,
                                                oldWorldP.Position.Y);
                        worldP.Scale = Vec3(thickness, oldWorldP.Scale.Y + thickness, 1);
                        entity.Transform.Update(worldP.Position, worldP.Scale, worldP.Orientation);
                        m_renderQueue.Submit("PixelFill", entity.hRender, entity.Transform);

                        // Bottom rectangle
                        worldP.Position.XY = Vec2(oldWorldP.Position.X, 
                                                oldWorldP.Position.Y - radius.Y);
                        worldP.Scale = Vec3(oldWorldP.Scale.X + thickness, thickness, 1);
                        entity.Transform.Update(worldP.Position, worldP.Scale, worldP.Orientation);
                        m_renderQueue.Submit("PixelFill", entity.hRender, entity.Transform);

                        // Reinstate the base state.
                        worldP = oldWorldP;
                        entity.hRender.SetColor(m_baseColor);
                    }
                }
                else
                {
                    m_renderQueue.Submit("Textured", entity.hRender, entity.Transform);
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
            if (EntityManager::HasComponent<Dragable2D>(entity))
            {
                auto& dragable = EntityManager::GetComponent<Dragable2D>(entity);
                dragable.DetachInputHandlers();
            }

            if (EntityManager::HasComponent<Clickable2D>(entity))
            {
                auto& clickable = EntityManager::GetComponent<Clickable2D>(entity);
                clickable.DetachInputHandlers();
            }
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