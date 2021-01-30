#include "Entity/EntityManager.h"

namespace Tange
{
    EntityManager& EntityManager::Get()
    {
        static EntityManager instance;
        return instance;
    }

    EntityManager::~EntityManager()
    {
        for (auto pComponent : m_componentSystems)
        {
            delete pComponent;
        }
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
        for (auto pComponent : m_componentSystems)
        {
            pComponent->DestroyEntity(entity);
        }
        entity = {};
    }

    template<typename T>
    void EntityManager::RegisterComponent()
    {
        // Make sure that the event was not already registered.
        ASSERT(!T::IsInitialized());
        
        if (!T::IsInitialized())
        {
            auto* pComponentArray = new ComponentArray<T, MaxEntityCount>();
            m_componentSystems.push_back(pComponentArray);
        }
    }

    template<typename T>
    T& EntityManager::AttachComponent(Entity entity)
    {
        auto* pComponents = GetComponentArray<T>();
        T& component = pComponents->GetComponent(entity);
        component.Entity = entity;
        return component;
    }

    template<typename T>
    T& EntityManager::GetComponent(Entity entity)
    {
        auto* pComponents = GetComponentArray<T>();
        T& component = pComponents->GetComponent(entity);
        // Make sure we are not referencing a entity that's not attached/stale.
        ASSERT(component.Entity == entity);
        return component;
    }

    template<typename T>
    bool EntityManager::HasComponent(Entity entity)
    {
        auto* pComponents = GetComponentArray<T>();
        T& component = pComponents->GetComponent(entity);
        return component.Entity == entity;
    }

    template <typename T>
    ComponentArray<T, EntityManager::MaxEntityCount>* EntityManager::GetComponentArray()
    {
        int x = T::GetIndex();
        return static_cast<ComponentArray<T, MaxEntityCount>*>(m_componentSystems.at(T::GetIndex()));
    }
}