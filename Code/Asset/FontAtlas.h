#pragma once

#include "Core/Common.h"
#include "Asset/Image.h"
#include "Platform/FileSystem.h"

namespace Tange
{
    #include <freetype/freetype.h>

    static const int32 GlyphCount = 128;

    struct GlyphInfo
    {
        // Texture coordinates for the glyph in the texture atlas.
        Vec2 MinTexCoords;
        Vec2 MaxTexCoords;
        Vec2 Size;
        // X and Y bearing.
        int32 OffsetX;
        int32 OffsetY;
        // How far to go for next glyph.
        int32 AdvanceX;
    };

    class FontAtlas
    {
        // Used for finding the glyph in the atlas and positioning
        // the glyph correctly.
        GlyphInfo m_glyphInfos[GlyphCount];
        
    public:
        int32 GlyphPixelSize;
        std::string FontName;

        // Builds a font atlas for the specificed font.
        // Returns the image for the atlas, which should be submitted to the GPU
        // as a texture (NOTE: it is a monochrome bitmap). Also builds the glyph
        // information which is used to place glyphs correctly and gets the texture
        // coordinates for the glyph.
        Image BuildFont(const FileData& fontFile, 
                        const std::string& fontName, 
                        int32 glyphSize = 16);

        // Grabs the glyph information for the specified character.
        const GlyphInfo& LookupGlyphInfo(char character) const;
    };
}
