#pragma once

#include "Core/Common.h"

namespace Tange
{
    union Entity
    {
        struct
        {
            // Cannot be 0.
            int16 Id;
            int16 Index;
        };
        int32 Packed;

        bool operator==(const Entity& operand) const
        {
            return (this->Packed == operand.Packed);
        }

        bool IsValid() const
        {
            return Id != 0;
        }
    };
}