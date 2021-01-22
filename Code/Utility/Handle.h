#pragma once

#include "Core/Common.h"

// NOTE: an id of 0 is considered invalid in this implementation.
// The template is there simply for type checking.

template <typename T>
class Handle
{
    public:
    
    Handle() = default;
    Handle(uint32 id, uint32 index)
        : m_uid(id), m_index(index)
    {
        ASSERT(id != 0);
    }
    
    uint32 GetId() const
    {
        return m_uid;
    }

    uint32 GetIndex() const
    {
        return m_index;
    }

    bool IsValid() const
    {
        return m_uid != 0;
    }
    
    private:
    
    // Unique id.
    uint32 m_uid;
    uint32 m_index;
};