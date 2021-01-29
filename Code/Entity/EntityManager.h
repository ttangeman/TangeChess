#pragma once

#include "Core/Common.h"
#include "Entity/Entity.h"
#include "Entity/IComponent.h"

namespace ECS
{
    // NOTE: Prefer to use handles. Typically, the methods that do not take
    // handles require a linear search to find the entity.
    class EntityManager
    {
        public:

        static const int32 MaxEntityCount = 128;

        static EntityManager& Get();

        EntityManager() = default;
        EntityManager operator=(const EntityManager&) = delete;
        EntityManager(const EntityManager&) = delete;

        // Provides an entity with an unique id.
        Entity RegisterEntity();

        void DestroyEntity(Entity& entity);

        template<typename T>
        void RegisterComponent();

        template<typename T>
        T& AttachComponent(Entity entity);

        template<typename T>
        T& GetComponent(Entity entity);

        template<typename T>
        bool HasComponent(Entity entity) const;

        private:

        // Used for assigning unique ids to enitties. Cannot be 0 as that
        // is the "invalid" entitiy.
        int32 m_entityAccumulator = 1;

        // Used for assigning unqiue ids to component types.
        int32 m_componentAccumulator = 0;

        // NOTE: Must not shrink/delete elements in order to 
        // maintain stable indices for component types.
        std::vector<std::unique_ptr<IComponentSystem>> m_componentSystems;
    };
}