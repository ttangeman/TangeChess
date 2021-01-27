#pragma once

#include "Core/Common.h"

// NOTE: an id of 0 is considered invalid in this implementation.
// The template is there simply for type checking.

template <typename T>
class Handle
{
    public:

    // Unique id.
    uint32 Uid;

    union
    {
        uint32 Index;
        struct
        {
            uint16 ChunkId; // Which chunk to index.
            uint16 ChunkIndex; // The index into the chunk.
        };
    };
    
    Handle() = default;

    Handle(uint32 uid, uint32 index)
        : Uid(uid), Index(index)
    {
        ASSERT(uid != 0);
    }

    Handle(uint32 uid, uint16 chunkId, uint16 chunkIndex)
        : Uid(uid), ChunkId(chunkId), ChunkIndex(chunkIndex)
    {
        ASSERT(uid != 0);
    }

    bool operator==(Handle<T> operand) const
    {
        return (this->Index == operand.Index && 
                this->Uid == operand.Uid) ? 
                true : false;
    }

    bool IsValid() const
    {
        return Uid != 0;
    }
};