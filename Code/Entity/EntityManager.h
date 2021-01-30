#pragma once

#include "Core/Common.h"
#include "Entity/Entity.h"
#include "Entity/Component.h"

// TODO: Need to add a free list to reuse destroyed entity slots.

namespace Tange
{  
    struct IComponentSystem
    {
        virtual void DestroyEntity(Entity entity) = 0;
    };

    template<typename T, int32 ArraySize>
    struct ComponentArray : IComponentSystem
    {
        std::array<T, ArraySize> Components;

        ComponentArray() = default;

        T& GetComponent(Entity entity)
        {
            return Components.at(entity.Index);
        }
    
        void DestroyEntity(Entity entity) override
        {
            Components.at(entity.Index).Entity = {};
        }
    };

    class EntityManager
    {
        // Used for assigning unique ids to enitties. Cannot be 0 as that
        // is the "invalid" entitiy.
        int32 m_entityAccumulator = 1;

        // NOTE: Must not shrink/delete elements in order to
        // maintain stable indices for component types.
        // Essentially a custom pool allocator for each component type.
        std::vector<IComponentSystem*> m_componentSystems;

    public:
        static const int32 MaxEntityCount = 128;

        static EntityManager& Get();
        ~EntityManager();
        EntityManager operator=(const EntityManager&) = delete;
        EntityManager(const EntityManager&) = delete;

        // Provides an entity with an unique id.
        Entity RegisterEntity();

        // Destroys any component references to the entity.
        void DestroyEntity(Entity& entity);

        // Registers a component with the entity system.
        template<typename T>
        void RegisterComponent();

        // Attaches a component to an entity.
        template<typename T>
        T& AttachComponent(Entity entity);

        // Tries to get a component for an entity.
        // NOTE: If an entity did not have the component then the
        // call will fail and trigger an assertion.
        template<typename T>
        T& GetComponent(Entity entity);

        // Checks to see if an entity has a component.
        template<typename T>
        bool HasComponent(Entity entity);

    private:
        template <typename T>
        ComponentArray<T, MaxEntityCount>* GetComponentArray();

        EntityManager() = default;
    };
}