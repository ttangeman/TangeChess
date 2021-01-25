#include "Asset/FontAtlas.h"

namespace Asset
{
    // Helper function to flip a glyph bitmap from bottom-up
    // to top-down bitmap. Just a copy of Asset::Image::FlipBitmap()
    // but for FreeType's struct.
    static void FlipBitmap(FT_Bitmap& bitmap)
    {
        usize size = bitmap.width * bitmap.rows;
        uint8* pCopy = (uint8*) malloc(size);
        Platform::Copy(pCopy, bitmap.buffer, size);
        
        // Point to the last row in the bitmap.
        usize sourceOffset = (bitmap.rows - 1) * bitmap.pitch;
        usize destOffset = 0;
        
        for (auto y = 0; y < bitmap.rows; y++) 
        {
            // Swap the bottom and top rows.
            Platform::Copy(bitmap.buffer + destOffset, pCopy + sourceOffset, bitmap.pitch);
            sourceOffset -= bitmap.pitch;
            destOffset += bitmap.pitch;
        }

        free(pCopy);
    }   

    // Based on: https://gist.github.com/baines/b0f9e4be04ba4e6f56cab82eef5008ff
    // Slightly modified to work with top-down bitmaps and to provide 0-1 UV coordinates.
    Image FontAtlas::BuildFont(const Platform::FileData& fontFile, int32 charSize)
    {
        FT_Library library;
        auto error = FT_Init_FreeType(&library);
        ASSERT(!error);
        
        FT_Face face;
        error = FT_New_Memory_Face(library, (const uint8*)fontFile.pData.get(), 
                                   fontFile.Size, 0, &face);
        ASSERT(error != FT_Err_Unknown_File_Format && !error);

        FT_Set_Char_Size(face, 0, charSize << 6, 96, 96);

        // quick and dirty max texture size estimate
        auto maxDimension = (1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(GlyphCount));
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
            m_glyphInfos[i].MinUV = Vec2(minX / (float)textureWidth, minY / (float)textureHeight);
            m_glyphInfos[i].MaxUV = Vec2(maxX / (float)textureWidth, maxY / (float)textureHeight);
            m_glyphInfos[i].XOffset = face->glyph->bitmap_left;
            m_glyphInfos[i].YOffset = face->glyph->bitmap_top;
            m_glyphInfos[i].XAdvance = face->glyph->advance.x >> 6;

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