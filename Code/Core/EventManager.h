#pragma once

#include "Core/Common.h"

namespace Core
{
    struct IEvent
    {
        inline static int32 EventIndex = 0;

        virtual ~IEvent()
        {
        }
    };

    struct Handler
    {
        std::function<void(const IEvent&)> Callback;
        std::string Name;

        Handler(const std::string& name, std::function<void(const IEvent&)> callback)
            : Name(name), Callback(callback)
        {
        }
    };

    class EventManager
    {
        public:

        static EventManager& Get();

        // Registers the event for possible handling.
        template<typename T>
        void RegisterEvent();

        // Registers a callback for an event.
        template<typename T>
        void BindHandler(const std::string& handlerName,
                                const std::function<void(const IEvent&)>& callback);

        // Detach a specific event type from a handler name.
        template<typename T>
        void DetachHandler(const std::string& handlerName);

        // Detaches all handlers registered under a name.
        void DetachAllHandlers(const std::string& handlerName);

        // Dispatches an event to all subscribed handlers.
        void Dispatch(const IEvent& event);
        // TODO: Could have a dispatch for a specific Handler::Name?

        private:

        // Assigns an index to a registered event.
        int32 m_eventAccumulator = 0;

        // NOTE: Unable to remove from the outer vector in order to maintain
        // index stability. The inner vector is used as a FIFO queue, so deleting
        // handlers is allowed.
        std::vector<std::vector<Handler>> m_eventHandlers;
    };
}