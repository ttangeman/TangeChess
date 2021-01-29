#pragma once

#include "Core/Common.h"
#include "World/Entity.h"

namespace World
{
    // NOTE: Special care needs to be taken to not overwrite the ComponentIndex,
    // so components should only be accessed through the entity manager. (Unfortunately,
    // the variable cannot be made const and initialized once because it is static).
    struct IComponent
    {
        // Used for indexing a component with a unique identifier, which
        // can then be used for indexing a ComponentArray in the entity manager.
        // NOTE: Set by EntityManager::RegisterComponent.
        inline static int32 ComponentIndex;

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