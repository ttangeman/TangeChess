#include "World/EntityManager.h"

namespace World
{
    void EntityManager::RegisterEntity(Entity& entity)
    {
        entity.Id = m_accumulator++;
    }

    std::optional<Entity> EntityManager::LookupEntity(Handle<Entity> hEntity) const
    {
        auto& entity = m_entities.at(hEntity.Index);
        if (entity.Id == hEntity.Uid)
        {
            return entity;
        }
        return std::nullopt;
    }

    Entity& EntityManager::GetEntity(Handle<Entity> hEntity)
    {
        auto& entity = m_entities.at(hEntity.Index);
        // Handle mismatch if false.
        ASSERT(entity.Id == hEntity.Uid);
        return entity;
    }

    std::optional<Entity> EntityManager::LookupEntity(int32 id) const
    {
        for (auto& it : m_entities)
        {
            if (it.Id == id)
            {
                return it;
            }
        }
        return std::nullopt;
    }

    Entity& EntityManager::GetEntity(int32 id)
    {
        for (auto& it : m_entities)
        {
            if (it.Id == id)
            {
                return it;
            }
        }
        // Couldn't find the entity!
        INVALID_CODE_PATH();
        return m_entities[0];
    }
}