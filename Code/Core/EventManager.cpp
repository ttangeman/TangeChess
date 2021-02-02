#include "Core/EventManager.h"

namespace Tange
{
    EventManager EventManager::s_instance;

    template<typename T>
    void EventManager::RegisterEvent()
    {
        // Make sure that the event was not already registered.
        ASSERT(!T::IsInitialized());

        if (!T::IsInitialized())
        {
            T::Initialize();
            std::vector<Handler> eventHandler;
            s_instance.m_eventHandlers.push_back(eventHandler);
        }
    }

    template<typename T>
    void EventManager::BindHandler(int32 id, const std::function<void(const IEvent&)>& callback)
    {
        ASSERT(T::IsInitialized());

        if (T::IsInitialized())
        {
            auto& handlers = s_instance.m_eventHandlers.at(T::GetIndex());
            handlers.emplace_back(Handler(id, callback));
        }
    }

    template<typename T>
    void EventManager::DetachHandler(int32 id)
    {
        auto& handlers = s_instance.m_eventHandlers.at(T::GetIndex());
        
        for (auto i = 0; i < handlers.size(); i++)
        {
            auto& it = handlers[i];
            if (it.Id == id)
            {
                handlers.erase(handlers.begin() + i);
                break;
            }
        }
    }

    void EventManager::DetachAllHandlers(int32 id)
    {   
        for (auto& handlers : s_instance.m_eventHandlers)
        {
            for (auto i = 0; i < handlers.size(); i++)
            {
                auto& it = handlers[i];
                if (it.Id == id)
                {
                    handlers.erase(handlers.begin() + i);
                    break;
                }
            }
        }
    }

    template<typename T>
    void EventManager::Dispatch(IEvent&& payload)
    {
        for (auto& it : s_instance.m_eventHandlers.at(T::GetIndex()))
        {
            it.Callback(payload);
        }
    }

    template <typename T>
    void EventManager::DispatchTo(int32 id, IEvent&& payload)
    {
        for (auto& it : s_instance.m_eventHandlers.at(T::GetIndex()))
        {
            if (it.Id == id)
            {
                it.Callback(payload);
            }
        }
    }
}