#pragma once

#include "Core/Common.h"
#include "Core/Event.h"

namespace Tange
{
    struct EventHandler
    {
        // The function to dispatch the event to.
        std::function<void(const IEvent&)> Callback;

        // Could be a random id or, more likely, an entity id, for example.
        // Either way, the id is used for detaching handlers and 
        // single dispatches.
        int32 Id;

        EventHandler(int32 id, std::function<void(const IEvent&)> callback)
            : Id(id), Callback(callback)
        {
        }
    };

    class EventManager
    {
        static EventManager s_instance;

        // NOTE: Unable to remove from the outer vector in order to maintain
        // index stability. The inner vector is used as a FIFO queue, so deleting
        // handlers is allowed.
        std::vector<std::vector<EventHandler>> m_eventHandlers;

        static bool IsRegisteredEvent(int32 index);

    public:
        EventManager(const EventManager&) = delete;
        void operator=(const EventManager&) = delete;

        // Registers a callback for an event.
        template<typename T>
        static void BindHandler(int32 id, const std::function<void(const IEvent&)>& callback);

        // Detach a specific event type from a handler id.
        template<typename T>
        static void DetachHandler(int32 id);

        // Detaches all handlers registered under an id.
        static void DetachAllHandlers(int32 id);

        // Dispatches an event to all subscribed handlers.
        template<typename T>
        static void Dispatch(IEvent&& payload);

        // Does a single dispatch to the id.
        // NOTE: Has to do a linear search for the correct id, so should
        // be avoided where possible.
        template<typename T>
        static void DispatchTo(int32 id, IEvent&& payload);

    private:
        EventManager() = default;
    };
}