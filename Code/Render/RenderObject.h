#pragma once

#include "Render/D3D11/D3D11Renderer.h"

namespace Render
{
    struct Vertex
    {
        Vec3 Position;
        Vec4 Color;
        Vec2 TexCoord;
        
        Vertex() = default;
    };
    
    // Implementation defined by render API.
    class RenderObject;
}