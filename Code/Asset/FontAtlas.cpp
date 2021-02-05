#include "Asset/FontAtlas.h"

namespace Tange
{
    // Helper function to flip a glyph bitmap from a bottom-up
    // to a top-down bitmap. Just a copy of Asset::Image::FlipImage()
    // but for FreeType's struct.
    static void FlipBitmap(FT_Bitmap& bitmap)
    {
        usize size = bitmap.width * bitmap.rows;
        uint8* pCopy = (uint8*)std::malloc(size);
        std::memcpy(pCopy, bitmap.buffer, size);
        
        // Point to the last row in the bitmap.
        usize sourceOffset = (bitmap.rows - 1) * bitmap.pitch;
        usize destOffset = 0;
        
        for (auto y = 0; y < bitmap.rows; y++) 
        {
            // Swap the bottom and top rows.
            std::memcpy(bitmap.buffer + destOffset, pCopy + sourceOffset, bitmap.pitch);
            sourceOffset -= bitmap.pitch;
            destOffset += bitmap.pitch;
        }

        std::free(pCopy);
    }   

    // Based on: https://gist.github.com/baines/b0f9e4be04ba4e6f56cab82eef5008ff
    // Slightly modified to work with top-down bitmaps and to provide 0-1 UV coordinates.
    Image FontAtlas::BuildFont(const FileData& fontFile, 
                               const std::string& fontName, 
                               int32 glyphSize)
    {
        GlyphPixelSize = glyphSize;
        FontName = fontName;

        FT_Library library;
        auto error = FT_Init_FreeType(&library);
        ASSERT(!error);
        
        FT_Face face;
        error = FT_New_Memory_Face(library, (const uint8*)fontFile.pData.get(), 
                                   fontFile.Size, 0, &face);
        ASSERT(error != FT_Err_Unknown_File_Format && !error);

        FT_Set_Char_Size(face, 0, glyphSize << 6, 96, 96);

        // Quick and dirty max texture size estimate.
        auto maxDimension = (1 + (face->size->metrics.height >> 6)) * std::ceilf(std::sqrtf(GlyphCount));
	    auto textureWidth = 1;

	    while(textureWidth < maxDimension) 
        {
            textureWidth <<= 1;
        }

	    auto textureHeight = textureWidth;

        // Monochrome bitmap for the atlas. To be submitted as a texture to the GPU.
        Image fontBitmap = {};
        fontBitmap.BytesPerPixel = 1;
        fontBitmap.Width = textureWidth;
        fontBitmap.Height = textureHeight;
        fontBitmap.Pitch = textureWidth;
        fontBitmap.pPixels = (uint8*)std::malloc(fontBitmap.GetSize());
        std::memset(fontBitmap.pPixels, 1, fontBitmap.GetSize());

        auto penX = 0;
        auto penY = 0;

        for (auto i = 0; i < GlyphCount; i++)
        {
            FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
            FT_Bitmap& bitmap = face->glyph->bitmap;
            // Flip the bottom-up bitmap to top-down.
            FlipBitmap(bitmap);
            
            if (penX + bitmap.width >= textureWidth)
            {
                penX = 0;
                penY += ((face->size->metrics.height >> 6) + 1);
            }

            for (auto row = 0; row < bitmap.rows; ++row)
            {
                for (auto col = 0; col < bitmap.width; ++col)
                {
                    auto x = penX + col;
                    auto y = penY + row;
                    fontBitmap.pPixels[y * textureWidth + x] = bitmap.buffer[row * bitmap.pitch + col];
                }
            }

            // These are the raw bitmap coordinates for the glyph.
            float minX = penX;
            float minY = penY;
            float maxX = penX + bitmap.width;
            float maxY = penY + bitmap.rows;
            // Convert down to a 0-1 range for the GPU.
            m_glyphInfos[i].MinTexCoords = Vec2(minX / (float)textureWidth, minY / (float)textureHeight);
            m_glyphInfos[i].MaxTexCoords = Vec2(maxX / (float)textureWidth, maxY / (float)textureHeight);
            // These are used for placing the glyph.
            m_glyphInfos[i].Size = Vec2(bitmap.width, bitmap.rows);
            m_glyphInfos[i].OffsetX = face->glyph->bitmap_left;
            m_glyphInfos[i].OffsetY = face->glyph->bitmap_top;
            m_glyphInfos[i].AdvanceX = face->glyph->advance.x >> 6;

            penX += bitmap.width + 1;
        }

        FT_Done_FreeType(library);

        return fontBitmap;
    }

    const GlyphInfo& FontAtlas::LookupGlyphInfo(char character) const
    {
        return m_glyphInfos[character];
    }
}