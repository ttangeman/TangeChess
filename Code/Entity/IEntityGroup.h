#pragma once

#include "Core/Common.h"
#include "Entity/Entity.h"

namespace Tange
{
    // Used for discretizing the world. It would be inefficient to operate
    // on all entities in a large scale world at once, so the world can instead
    // be grouped into small chunk of entities.
    class IEntityGroup
    {
    protected:
        std::vector<Entity> m_entities;

    public:
        virtual void Update(float timeStep) = 0;
        virtual void Render() = 0;
    };
}