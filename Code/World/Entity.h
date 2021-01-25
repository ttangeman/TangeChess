#pragma once

#include "Core/Common.h"
#include "Render/RenderObject.h"

typedef int32 EntityId;

namespace World
{
    class Entity
    {
        public:

        EntityId Id;
        // TODO: This should be a render handle of some sort!
        Render::RenderObject& RenderObject;
        Vec3 Position;
        Vec3 Orientation;
        Vec3 Scale;

        virtual void OnUpdate();
    };
}