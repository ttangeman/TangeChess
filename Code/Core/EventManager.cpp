#include "Core/EventManager.h"

namespace Tange
{
    EventManager& EventManager::Get()
    {
        static EventManager instance;
        return instance;
    }

    template<typename T>
    void EventManager::RegisterEvent()
    {
        // Make sure that the event was not already registered.
        ASSERT(!T::IsInitialized());
        T::Initialize();
        std::vector<Handler> eventHandler;
        m_eventHandlers.push_back(eventHandler);
    }

    template<typename T>
    void EventManager::BindHandler(int32 id, const std::function<void(const IEvent&)>& callback)
    {
        ASSERT(T::IsInitialized());
        auto& handlers = m_eventHandlers.at(T::GetIndex());
        handlers.emplace_back(Handler(id, callback));
    }

    template<typename T>
    void EventManager::DetachHandler(int32 id)
    {
        auto& handlers = m_eventHandlers.at(T::GetIndex());
        bool found = false;

        for (auto i = 0; i < handlers.size(); i++)
        {
            auto& it = handlers[i];
            if (it.Id == id)
            {
                handlers.erase(handlers.begin() + i);
                found = true;
                break;
            }
        }

        ASSERT(found);
    }

    void EventManager::DetachAllHandlers(int32 id)
    {   
        for (auto& handlers : m_eventHandlers)
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
    void EventManager::Dispatch(const IEvent& payload)
    {
        for (auto& it : m_eventHandlers.at(T::GetIndex()))
        {
            it.Callback(payload);
        }
    }

    template <typename T>
    void EventManager::DispatchTo(int32 id, const IEvent& payload)
    {
        for (auto& it : m_eventHandlers.at(T::GetIndex()))
        {
            if (it.Id == id)
            {
                it.Callback(payload);
            }
        }
    }
}