#pragma once

#include "Core/Common.h"
#include "Render/Renderer.h"
#include "Render/RenderHandle.h"

namespace Tange
{
    struct Entity
    {
        int32 Id;
        int32 Index;
        RenderHandle hRender = {};
        Transform Transform = {};

        bool operator==(const Entity& operand) const
        {
            return (this->Id == operand.Id);
        }

        bool IsValid() const
        {
            return Id != 0;
        }
    };
}