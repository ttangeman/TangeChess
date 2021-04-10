#include "UserInterface/TextLine.h"

namespace Tange
{
    TextLine::TextLine(const GuiConfig& config, const std::string& text, Vec2 position)
        : TextLine(text, *config.pFontAtlas, position, 
                   config.FontPixelHeight, config.FontColor)
    {
    }

    TextLine::TextLine(const std::string& text, const FontAtlas& atlas, 
                       Vec2 position, float pixelHeight, Vec4 color)
        : m_text(text)
    {
        if (text.empty()) return;
        
        float scale = pixelHeight / atlas.GlyphPixelSize;
        float textLineWidth = 0;

        // Loop once over the text to compute a bounding rectangle
        // for the line(s) of text.
        for (auto i = 0; i < text.length(); i++)
        {
            const auto& glyphInfo = atlas.LookupGlyphInfo(text[i]);
            // NOTE: XAdvance accounts for spaces, glyphInfo.Size does not!
            textLineWidth += glyphInfo.AdvanceX * scale;
        }

        // TODO: Text wrapping if it goes offscreen!
        ASSERT(textLineWidth <= Renderer::GetDrawRegion().Width);

        // Center the bounding box at the specified position.
        auto adjustedP = Vec2(position.X - (textLineWidth / 2.0),
                              position.Y - (pixelHeight / 2.0));

        Quad* pQuads = (Quad*)std::malloc(sizeof(Quad) * text.length());

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

        // TODO: Get a proper id from ResourceManager!
        // This is not sufficient because we could very easily have
        // collisions.
        ResourceManager::SubmitMesh(text, pQuads, 
                                    Quad::VerticeCount * text.length(), 
                                    sizeof(Vertex));
        std::free(pQuads);

        m_hRender.AttachMesh(text);
        m_hRender.AttachTexture(atlas.FontName);
        m_hRender.SetColor(Vec4(0, 0, 0, 1));
        m_transform.WindowOrthographic();
        m_scale = Vec2(1, 1);

        // TODO: Get a proper id from event manager!
        EventManager::BindHandler<WindowResized>(1,
        [this](const IEvent& event)
        {
            const auto& resizeEvent = static_cast<const WindowResized&>(event);

            auto tScale = Vec2(m_scale.Width / resizeEvent.CurrentWidth,
                               m_scale.Height / resizeEvent.CurrentHeight);
            m_scale = Vec2(resizeEvent.DesiredWidth * tScale.Width,
                            resizeEvent.DesiredHeight * tScale.Height);
            
            auto tPosition = Vec2(m_position.Width / resizeEvent.CurrentWidth,
                                  m_position.Height / resizeEvent.CurrentHeight);
            m_position = Vec2(resizeEvent.DesiredWidth * tPosition.Width,
                              resizeEvent.DesiredHeight * tPosition.Height);
        });
    }

    void TextLine::Render(const RenderQueue& queue)
    {
        if (m_visible)
        {
            queue.Submit("Textured", m_hRender, m_transform);
        }
    }
}