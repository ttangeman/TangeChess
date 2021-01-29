#include "Entity/EntityManager.h"

namespace ECS
{
    EntityManager& EntityManager::Get()
    {
        static EntityManager instance;
        return instance;
    }

    Entity EntityManager::RegisterEntity()
    {
        Entity entity = {};
        entity.Id = m_entityAccumulator++;
        entity.Index = entity.Id - 1;
        ASSERT(entity.Id < MaxEntityCount);
        return entity;
    }

    void EntityManager::DestroyEntity(Entity& entity)
    {
        for (auto& it : m_componentSystems)
        {
            it->DestroyEntity(entity);
        }
        entity = {};
    }

    template<typename T>
    void EntityManager::RegisterComponent()
    {
        ComponentArray<T, MaxEntityCount> componentArray;
        int x = T::ComponentIndex;
        // Make sure that the event was not already registered.
        //ASSERT(T::ComponentIndex == 0);
        T::ComponentIndex = m_componentAccumulator++;

        auto pComponentSystem = std::make_unique<ComponentArray<T, MaxEntityCount>>(componentArray);
        m_componentSystems.push_back(std::move(pComponentSystem));
    }

    template<typename T>
    T& EntityManager::AttachComponent(Entity entity)
    {
        T* component = (T*) m_componentSystems.at(T::ComponentIndex)->GetComponent(entity);
        component->Entity = entity;
        return *component;
    }

    template<typename T>
    T& EntityManager::GetComponent(Entity entity)
    {
        T* component = (T*) m_componentSystems.at(T::ComponentIndex)->GetComponent(entity);
        // Make sure we are not referencing a entity that's not attached/stale.
        ASSERT(component->Entity == entity);
        return *component;
    }

    template<typename T>
    bool EntityManager::HasComponent(Entity entity) const
    {
        T* component = (T*) m_componentSystems.at(T::ComponentIndex)->GetComponent(entity);
        return component->Entity == entity;
    }
}