#pragma once

#include "Core/Common.h"
#include "Render/RenderObject.h"

typedef int32 EntityId;

namespace World
{
    struct Entity
    {
        EntityId Id;
        Render::RenderObject RenderObject;
        Vec3 Position;
        Vec3 Orientation;
        Vec3 Scale;
    };
}