#include "Entity/EntityManager.h"

namespace Tange
{
    EntityManager EntityManager::s_instance;

    EntityManager::~EntityManager()
    {
        for (auto pComponent : s_instance.m_componentContainers)
        {
            delete pComponent;
            pComponent = nullptr;
        }
    }

    Entity EntityManager::RegisterEntity()
    {
        Entity entity = {};
        entity.Id = s_instance.m_entityAccumulator++;
        entity.Index = entity.Id - 1;
        ASSERT(entity.Id < g_MaxEntityCount);
        return entity;
    }

    void EntityManager::DestroyEntity(Entity& entity)
    {
        for (auto pComponent : s_instance.m_componentContainers)
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
            auto* pComponentArray = new ComponentArray<T>();
            s_instance.m_componentContainers.push_back(pComponentArray);
        }
    }

    template<typename T>
    T& EntityManager::AttachComponent(Entity entity)
    {
        auto* pComponents = CastComponentArray<T>();
        T& component = pComponents->GetComponent(entity);
        component.BoundEntity = entity;
        return component;
    }

    template<typename T>
    T& EntityManager::GetComponent(Entity entity)
    {
        auto* pComponents = CastComponentArray<T>();
        T& component = pComponents->GetComponent(entity);
        // Make sure we are not referencing a entity that's not attached/stale.
        ASSERT(component.BoundEntity == entity);
        return component;
    }
    
    template<typename T>
    bool EntityManager::HasComponent(Entity entity)
    {
        if (entity.IsValid())
        {
            auto* pComponents = CastComponentArray<T>();
            T& component = pComponents->GetComponent(entity);
            return component.BoundEntity == entity;
        }
        else
        {
            return false;
        }
    }

    template <typename T>
    ComponentArray<T>* EntityManager::CastComponentArray()
    {
        return static_cast<ComponentArray<T>*>(s_instance.m_componentContainers.at(T::GetIndex()));
    }
}