#pragma once

#include "Core/Common.h"
#include "Asset/FontAtlas.h"

namespace Tange
{
    struct GuiConfig
    {
        FontAtlas* pFontAtlas;

        Vec4 FontColor = Vec4(1, 1, 1, 1);
        Vec4 OutlineColor = Vec4(1, 1, 1, 0.8);

        float FontPixelHeight = 12;
        float OutlineThickness = 2;
    };
}