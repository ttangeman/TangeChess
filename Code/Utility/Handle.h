#pragma once

#include "Core/Common.h"

// NOTE: an id of 0 is considered invalid in this implementation.

template <typename T>
class Handle
{
    public:
    
    Handle() = default;
    Handle(uint64 id, usize index)
        : m_id(id), m_index(index)
    {
        Assert(id != 0);
    }
    
    uint64 GetId() const
    {
        return m_id;
    }

    usize GetIndex() const
    {
        return m_index;
    }

    bool IsValid() const
    {
        return m_id != 0;
    }
    
    private:
    
    uint64 m_id;
    usize m_index;
};