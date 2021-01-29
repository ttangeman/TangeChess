#include "Core/EventManager.h"

namespace Core
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
        ASSERT(T::EventIndex <= m_eventAccumulator);
        T::EventIndex = m_eventAccumulator++;

        std::vector<Handler> eventHandler;
        m_eventHandlers.push_back(eventHandler);
    }

    template<typename T>
    void EventManager::BindHandler(const std::string& handlerName,
                                   const std::function<void(const IEvent&)>& callback)
    {
        auto& handlers = m_eventHandlers.at(T::EventIndex);
        handlers.emplace_back(Handler(handlerName, callback));
    }

    template<typename T>
    void EventManager::DetachHandler(const std::string& handlerName)
    {
        auto& handlers = m_eventHandlers.at(T::EventIndex);
        bool found = false;

        for (auto i = 0; i < handlers.size(); i++)
        {
            auto& it = handlers[i];
            if (it.Name == handlerName)
            {
                handlers.erase(handlers.begin() + i);
                found = true;
                break;
            }
        }

        ASSERT(found);
    }

    void EventManager::DetachAllHandlers(const std::string& handlerName)
    {   
        for (auto& handlers : m_eventHandlers)
        {
            for (auto i = 0; i < handlers.size(); i++)
            {
                auto& it = handlers[i];
                if (it.Name == handlerName)
                {
                    handlers.erase(handlers.begin() + i);
                    break;
                }
            }
        }
    }
    
    void EventManager::Dispatch(const IEvent& event)
    {
        for (auto& it : m_eventHandlers.at(event.EventIndex))
        {
            it.Callback(event);
        }
    }
}