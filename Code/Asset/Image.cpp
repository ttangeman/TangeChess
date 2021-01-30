#include "Asset/Image.h"

namespace Tange
{
    void Image::LoadBMP(const std::string& fileName)
    {
        pPixels = stbi_load("Data/Pieces.bmp", &Width, &Height, &BytesPerPixel, 0);
        Pitch = Width * BytesPerPixel;
        // NOTE: BMP's are BGRA and bottom-up images, we want RGBA and top-down.
        FlipImage();
        SwapBGRAToRGBA();
    }

    void Image::FreePixels()
    {
        std::free(pPixels);
    }

    usize Image::GetSize() const
    {
        return Width * Height * BytesPerPixel;
    }

    void Image::FlipImage()
    {
        usize size = GetSize();
        uint8* pCopy = (uint8*)std::malloc(size);
        std::memcpy(pCopy, pPixels, size);
        
        // Point to the last row in the bitmap.
        usize sourceOffset = (Height - 1) * Pitch;
        usize destOffset = 0;
        
        for (auto y = 0; y < Height; y++) 
        {
            // Swap the bottom and top rows.
            std::memcpy(pPixels + destOffset, pCopy + sourceOffset, Pitch);
            sourceOffset -= Pitch;
            destOffset += Pitch;
        }

        std::free(pCopy);
    }

    void Image::SwapBGRAToRGBA()
    {
        ASSERT(BytesPerPixel == 4);
    
        uint32* pBitmap = (uint32*)pPixels;
    
        for (auto y = 0; y < Height; y++) 
        {
            for (auto x = 0; x < Width; x++) 
            {
                uint32& pixel = pBitmap[x + (y * Width)];
            
                uint8 blue = (uint8)(pixel);
                uint8 green = (uint8)(pixel >> 8);
                uint8 red = (uint8)(pixel >> 16);
                uint8 alpha = (uint8)(pixel >> 24);
            
                pixel = (red) |
                        (green << 8) |
                        (blue << 16) |
                        (alpha << 24);
            }
        }
    }
}