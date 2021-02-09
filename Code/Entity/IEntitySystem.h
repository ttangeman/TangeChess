#pragma once

#include "Core/Common.h"

namespace Tange
{
    // An entity system contains no data. It is simply responsible
    // for operating on sets of components.
    class IEntitySystem
    {
    public:
        virtual void Update(float timeStep) = 0;
    };
}