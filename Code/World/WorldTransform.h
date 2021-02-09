#pragma once

#include "Core/Common.h"
#include "Entity/Component.h"

namespace Tange
{
    struct WorldTransform : public Component<WorldTransform>
    {
        Vec3 Position = Vec3(0, 0, 1);
        Vec3 Scale = Vec3(1, 1, 1);
        Vec3 Orientation = Vec3(0, 0, 0);
    };
}