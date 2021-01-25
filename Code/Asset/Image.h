#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_BMP
#include "../Libraries/stb_image.h"

#include "Core/Common.h"
#include "Platform/PlatformWrapper.h"

namespace Asset
{
    class Image
    {
        public:

        uint8* pPixels;
        int32 Width;
        int32 Height;
        int32 BytesPerPixel;
        int32 Pitch;

        // Loads a Windows bitmap.
        void LoadBMP(const std::string& fileName);

        // Frees the pixel pointer.
        void FreePixels();

        usize GetSize() const;

        // Used for flipping the direction of certain file formats to bottom-down image. 
        // (Such as BMP from top-up to bottom-down).
        void FlipImage();

        // Used for swapping the endianess of pixel colors.
        void SwapBGRAToRGBA();
    };
}