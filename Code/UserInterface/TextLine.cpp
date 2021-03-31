#include "UserInterface/TextLine.h"

namespace Tange
{
    TextLine::TextLine()
    {
    }

    void TextLine::Destroy()
    {
        EventManager::DetachHandler<WindowResized>(1);
    }

    void TextLine::Update()
    {
        m_transform.Update(Vec3(m_position, 1), 
                    Vec3(m_scale, 1), 
                    Vec3(0, 0 , 0));
    }

    void TextLine::Render(const RenderQueue& queue)
    {
        queue.Submit("Textured", m_hRender, m_transform);
    }

    void TextLine::SetText(const FontAtlas& atlas, const std::string& text, 
                           Vec2 position, float pixelHeight, Vec4 color)
    {
        if (text.empty()) return;

        m_transform.WindowOrthographic();
        m_position = position;
        
        float scale = pixelHeight / atlas.GlyphPixelSize;
        m_scale = Vec2(scale, scale);

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

        // TODO: Get a proper id from ResourceManager!
        // This is not sufficient because we could very easily have
        // collisions.
        ResourceManager::SubmitMesh(text, pQuads, 
                                    Quad::VerticeCount * text.length(), 
                                    sizeof(Vertex));
        free(pQuads);

        m_hRender.AttachMesh(text);
        m_hRender.AttachTexture(atlas.FontName);
    }
}