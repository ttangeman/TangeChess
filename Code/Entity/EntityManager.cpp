#include "Entity/EntityManager.h"

namespace Tange
{
    EntityManager EntityManager::s_instance;

    EntityManager::~EntityManager()
    {
        for (auto pComponent : s_instance.m_componentSystems)
        {
            delete pComponent;
        }
    }

    Entity EntityManager::RegisterEntity()
    {
        Entity entity = {};
        entity.Id = s_instance.m_entityAccumulator++;
        entity.Index = entity.Id - 1;
        ASSERT(entity.Id < MaxEntityCount);
        return entity;
    }

    void EntityManager::DestroyEntity(Entity& entity)
    {
        for (auto pComponent : s_instance.m_componentSystems)
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
            s_instance.m_componentSystems.push_back(pComponentArray);
        }
    }

    template<typename T>
    T& EntityManager::AttachComponent(Entity entity)
    {
        auto* pComponents = CastComponentArray<T>();
        T& component = pComponents->GetComponent(entity);
        component.Entity = entity;
        return component;
    }

    template<typename T>
    T& EntityManager::GetComponent(Entity entity)
    {
        auto* pComponents = CastComponentArray<T>();
        T& component = pComponents->GetComponent(entity);
        // Make sure we are not referencing a entity that's not attached/stale.
        ASSERT(component.Entity == entity);
        return component;
    }
    
    template<typename T>
    bool EntityManager::HasComponent(Entity entity)
    {
        auto* pComponents = CastComponentArray<T>();
        T& component = pComponents->GetComponent(entity);
        return component.Entity == entity;
    }

    template <typename T>
    ComponentArray<T, EntityManager::MaxEntityCount>* EntityManager::CastComponentArray()
    {
        return static_cast<ComponentArray<T, MaxEntityCount>*>(s_instance.m_componentSystems.at(T::GetIndex()));
    }
}