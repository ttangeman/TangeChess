#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_BMP
#include "../Libraries/stb_image.h"

#include "Core/Common.h"
#include "Platform/PlatformWrapper.h"

namespace Tange
{
    class Image
    {
    public:
        uint8* pPixels;
        int32 Width;
        int32 Height;
        int32 BytesPerPixel;
        int32 Pitch;

        Image() = default;
        ~Image();
        Image(Image&& other);

        // Loads a Windows bitmap.
        void LoadBMP(const std::string& fileName);

        usize GetSize() const;

        // Used for flipping the direction of certain file formats to a top-down image. 
        // (Such as BMP from top-up to bottom-down).
        void FlipImage();

        // Used for swapping the endianess of pixel colors.
        void SwapBGRAToRGBA();

    private:
        // Frees the pixel pointer.
        void FreePixels();
    };
}