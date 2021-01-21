#pragma once

#include "Core/Common.h"
#include "Platform/PlatformWrapper.h"

namespace Render
{
    // These classes must be implemented by the render API.
    struct Shader;
    struct Texture;
    class ResourceManager; // Singleton.
}