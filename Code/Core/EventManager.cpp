#include "Core/EventManager.h"

namespace Tange
{
    EventManager EventManager::s_instance;

    bool EventManager::IsRegisteredEvent(int32 index)
    {
        // The index should never be greater than the number of handlers.
        ASSERT(index <= s_instance.m_eventHandlers.size());

        if (index == s_instance.m_eventHandlers.size())
        {
            return false;
        }
        return true;
    }

    template<typename DerivedEvent>
    void EventManager::BindHandler(int32 id, const std::function<void(const IEvent&)>& callback)
    {
        if (!IsRegisteredEvent(DerivedEvent::GetIndex()))
        {
            // Lazily register a vector of handlers if it is not registered.
            // The index gets lazily initialized in the if-statement too, 
            // if it was never before used.
            s_instance.m_eventHandlers.emplace_back(std::vector<EventHandler> {});
        }  

        auto& handlers = s_instance.m_eventHandlers.at(DerivedEvent::GetIndex());
        handlers.emplace_back(EventHandler(id, callback));
    }

    template<typename DerivedEvent>
    void EventManager::DetachHandler(int32 id)
    {
        // Make sure that we have handlers to actually detach.
        // This could probably be omitted, as the m_eventHandlers would throw
        // an out_of_bounds exception.
        ASSERT(IsRegisteredEvent(DerivedEvent::GetIndex()));

        if (IsRegisteredEvent(DerivedEvent::GetIndex()))
        {
            auto& handlers = s_instance.m_eventHandlers.at(DerivedEvent::GetIndex());
            
            for (auto i = 0; i < handlers.size(); i++)
            {
                auto& it = handlers[i];
                
                // NOTE: An id can have multiple of the same handler bound,
                // so this if does not break the loop.
                if (it.Id == id)
                {
                    handlers.erase(handlers.begin() + i);
                }
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

                // NOTE: An id can have multiple of the same handler bound,
                // so this if does not break the loop.
                if (it.Id == id)
                {
                    handlers.erase(handlers.begin() + i);
                }
            }
        }
    }

    template<typename DerivedEvent>
    void EventManager::Dispatch(IEvent&& payload)
    { 
        if (IsRegisteredEvent(DerivedEvent::GetIndex()))
        {
            auto& handler = s_instance.m_eventHandlers.at(DerivedEvent::GetIndex());

            for (auto i = 0; i < handler.size(); i++)
            {
                auto& it = handler[i];
                ASSERT(it.Callback); 
                it.Callback(payload);
            }
        }  
        else
        {
            // Lazily register a vector of handlers if it is not registered.
            // The index gets lazily initialized in the if-statement too, 
            // if it was never before used.
            s_instance.m_eventHandlers.emplace_back(std::vector<EventHandler> {});
        }
    }

    template <typename DerivedEvent>
    void EventManager::DispatchTo(int32 id, IEvent&& payload)
    {
        if (!IsRegisteredEvent(DerivedEvent::GetIndex()))
        {
            for (auto& it : s_instance.m_eventHandlers.at(DerivedEvent::GetIndex()))
            {
                if (it.Id == id)
                {
                    it.Callback(payload);
                }
            }
        }  
        else
        {
            // Lazily register a vector of handlers if it is not registered.
            // The index gets lazily initialized in the if-statement too, 
            // if it was never before used.
            s_instance.m_eventHandlers.emplace_back(std::vector<EventHandler> {});
        }
    }
}