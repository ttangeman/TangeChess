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
            
            // NOTE: An id can have multiple of the same handler bound,
            // so this if does not break the loop.
            if (it.Id == id)
            {
                handlers.erase(handlers.begin() + i);
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

    template<typename T>
    void EventManager::Dispatch(IEvent&& payload)
    {
        // Robustness: A strange bug occured here where a foreach loop
        // picked up erased elements from DetachAllHandlers in one of
        // the handler vectors, even though I could SEE that the vector
        // had size 0 in the debugger (even tried shrink_to_fit).
        // Probably an odd race condition with input messages because
        // it doesn't happen in other cases (?). Manually looping 
        // through the vector seems to fix it??? 
        auto& handler = s_instance.m_eventHandlers.at(T::GetIndex());
        for (auto i = 0; i < handler.size(); i++)
        {
            auto& it = handler[i];
            ASSERT(it.Callback); 
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