#pragma once

#include "Core/Common.h"
#include "Asset/Image.h"
#include "Platform/FileManager.h"

namespace Asset
{
    #include <freetype/freetype.h>

    static const int32 GlyphCount = 128;

    struct GlyphInfo
    {
        // Texture coordinates for the glyph in the texture atlas.
        Vec2 MinTexCoords;
        Vec2 MaxTexCoords;
        // X and Y bearing.
        int32 OffsetX;
        int32 OffsetY;
        // How far to go for next glyph.
        int32 AdvanceX;
    };

    class FontAtlas
    {
        public:

        // Builds a font atlas for the specificed font.
        // Returns the image for the atlas, which should be submitted to the GPU
        // as a texture (NOTE: it is a monochrome bitmap). Also builds the glyph
        // information which is used to place glyphs correctly and gets the texture
        // coordinates for the glyph.
        Image BuildFont(const Platform::FileData& fontFile, int32 glyphSize = 128);

        // Grabs the glyph information for the specififed character.
        const GlyphInfo& LookupGlyphInfo(char character) const;

        private:

        // Used for finding the glyph in the atlas and positioning
        // the glyph correctly.
        GlyphInfo m_glyphInfos[GlyphCount];
    };
}
