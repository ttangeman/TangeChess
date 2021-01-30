#pragma once

#include "Core/Common.h"
#include "Entity/Entity.h"

namespace Tange
{ 
    static int32 _ComponentAccumulator = 0;

    // The template pattern is needed to make each inheritance
    // of Component to be unique for m_componentIndex.
    template<class T>
    class Component
    {        
        // Each component has a unique index.
        static inline int32 m_componentIndex = -1;

    public:
        // Used for validating entities and binding entity ids
        // to event handlers.
        Entity Entity;

        static int32 GetIndex()
        {
            return m_componentIndex;
        }

        static bool IsInitialized()
        {
            return m_componentIndex != -1;
        }

        Component()
        {
            if (!IsInitialized())
            {
                m_componentIndex = _ComponentAccumulator++;
            }
        }

    protected:
        ~Component()
        {
        }
    };
}