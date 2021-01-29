#pragma once

#include "Core/Common.h"
#include "Entity/Entity.h"

namespace ECS
{
    // Every component needs a inline static int32 ComponentIndex!
    struct IComponent
    {
        // Used for validating entities.
        Entity Entity;
    };

    struct IComponentSystem
    {
        // HACK: This needs to be a void* because we can't templatize.
        virtual void* GetComponent(Entity entity) = 0;
        virtual void DestroyEntity(Entity entity) = 0;
    };

    template<typename T, int32 ArraySize>
    struct ComponentArray : IComponentSystem
    {
        std::array<T, ArraySize> Components;

        ComponentArray()
        {
            Components.fill({});
        }

        void* GetComponent(Entity entity) override
        {
            return Components.data() + entity.Index;
        }
    

        void DestroyEntity(Entity entity) override
        {
            Components.at(entity.Index).Entity = {};
        }
    };
}