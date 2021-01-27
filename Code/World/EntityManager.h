#pragma once

#include "Core/Common.h"
#include "World/Entity.h"

namespace World
{
    class EntityManager
    {
        public:

        static EntityManager& GetInstance();

        // Provides an entity with a unique id and registers it.
        void RegisterEntity(Entity& entity);

        // Removes the entity from the registry.
        void DestroyEntity(int32 id);

        std::optional<Entity> LookupEntity(Handle<Entity> hEntity) const;
        Entity& GetEntity(Handle<Entity> hEntity);

        // NOTE: These have to do a linear search to find the entity!
        // Prefer to use handles.
        std::optional<Entity> LookupEntity(int32 id) const;
        Entity& GetEntity(int32 id);

        private:

        // Using for assigning unique ids to enitties. Cannot be 0 as that
        // is the "invalid" entitiy.
        int32 m_accumulator = 1;

        // Stores all entities (TODO: Change to chunks).
        std::array<Entity, 1024> m_entities;
    };
}