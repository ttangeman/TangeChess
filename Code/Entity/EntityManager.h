#pragma once

#include "Core/Common.h"
#include "Entity/Component.h"

// TODO: Need to add a free list to reuse destroyed entity slots.

namespace Tange
{  
    struct IComponentSystem
    {
        IComponentSystem() = default;

        virtual ~IComponentSystem()
        {
        }

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
            Components.at(entity.Id).Release();
        }
    };

    class EntityManager
    {
        static EntityManager s_instance;

        // Used for assigning unique ids to enitties. Cannot be 0 as that
        // is the "invalid" entitiy.
        int32 m_entityAccumulator = 1;

        // NOTE: Must not shrink/delete elements in order to
        // maintain stable indices for component types.
        // Essentially a custom pool allocator for each component type.
        std::vector<IComponentSystem*> m_componentSystems;

    public:
        static const int32 MaxEntityCount = 128;

        ~EntityManager();
        EntityManager operator=(const EntityManager&) = delete;
        EntityManager(const EntityManager&) = delete;

        // Provides an entity with an unique id.
        static Entity RegisterEntity();

        // Destroys any component references to the entity.
        static void DestroyEntity(Entity& entity);

        // Registers a component with the entity system.
        template<typename T>
        static void RegisterComponent();

        // Attaches a component to an entity.
        template<typename T>
        static T& AttachComponent(Entity entity);

        // Tries to get a component for an entity.
        // NOTE: If an entity did not have the component then the
        // call will fail and trigger an assertion.
        template<typename T>
        static T& GetComponent(Entity entity);

        // Checks to see if an entity has a component.
        template<typename T>
        static bool HasComponent(Entity entity);

    private:
        // Helper for casting to the correct component array type.
        template <typename T>
        static ComponentArray<T, MaxEntityCount>* CastComponentArray();

        EntityManager() = default;
    };
}