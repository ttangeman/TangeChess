#pragma once

#include "Core/Common.h"
#include "Core/IEvent.h"

namespace Tange
{
    struct Handler
    {
        // The function to dispatch the event to.
        std::function<void(const IEvent&)> Callback;

        // Could be a random id or an entity id, for example.
        // Either way, the id is used for detaching handlers and 
        // single dispatches.
        int32 Id;

        Handler(int32 id, std::function<void(const IEvent&)> callback)
            : Id(id), Callback(callback)
        {
        }
    };

    class EventManager
    {
        // NOTE: Unable to remove from the outer vector in order to maintain
        // index stability. The inner vector is used as a FIFO queue, so deleting
        // handlers is allowed.
        std::vector<std::vector<Handler>> m_eventHandlers;

    public:
        static EventManager& Get();
        EventManager(const EventManager&) = delete;
        void operator=(const EventManager&) = delete;

        // Registers the event for possible handling.
        template<typename T>
        void RegisterEvent();

        // Registers a callback for an event.
        template<typename T>
        void BindHandler(int32 id, const std::function<void(const IEvent&)>& callback);

        // Detach a specific event type from a handler name.
        template<typename T>
        void DetachHandler(int32 id);

        // Detaches all handlers registered under a name.
        void DetachAllHandlers(int32 id);

        // Dispatches an event to all subscribed handlers.
        template<typename T>
        void Dispatch(IEvent&& payload);

        // Does a single dispatch to the id.
        template<typename T>
        void DispatchTo(int32 id, IEvent&& payload);

    private:
        EventManager() = default;
    };
}