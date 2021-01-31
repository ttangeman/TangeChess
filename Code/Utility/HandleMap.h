#pragma once

#include "Core/Common.h"
#include "Utility/Handle.h"

template <class T, int32 ResourceCount>
class HandleMap
{
    // Used for assigning unique id's to resources. Cannot be 0.
    uint32 m_accumulator = 1;

public:
    std::array<T, ResourceCount> Resources;
    std::unordered_map<std::string, Handle<T>> hResources;

    HandleMap()
    {
        Resources.fill({});
        hResources.reserve(ResourceCount);
    }

    // NOTE: T _must_ have an Id field which is modified by this function!
    // Pushes the element into the vector and maps a handle to the data
    // under the element name.
    void PushResource(const std::string& resourceName, T&& resource)
    {
        resource.Id = m_accumulator++;

        int32 index = -1;

        // TODO: Maybe can do better than a linear search. A free list would
        // be faster but would require more memory, for example.
        for (auto i = 0; i < Resources.size(); i++)
        {
            const auto& it = Resources[i];
            if (it.Id == 0)
            {
                Resources[i] = resource;
                index = i;
                break;
            }
        }

        // In the future, I might want to evict resources that are not used
        // (but this would require some kind of reference counting on the handle).
        // The array is full if this is -1!
        ASSERT(index != -1);

        hResources.emplace(
            std::make_pair(resourceName, Handle<T>(resource.Id, index))
        );
    }

    void ReleaseResource(const std::string& resourceName)
    {
        // Clear to zero is considered "free" because the id cannot be
        // zero for resources.
        //Resources.at(GetResourceHandle(resourceName).Index) = {};
        hResources.erase(resourceName);
    }

    Handle<T> GetResourceHandle(const std::string& resourceName) const
    {
        return hResources.at(resourceName);
    }

    const T& LookupResource(Handle<T> hResource) const
    {
        const T& resource = Resources.at(hResource.Index);
        // Assert that we are not referencing a stale handle.
        ASSERT(resource.Id == hResource.Uid);
        return resource;
    }

    T& GetResource(Handle<T> hResource)
    {
        T& resource = Resources.at(hResource.Index);
        // Assert that we are not referencing a stale handle.
        ASSERT(resource.Id == hResource.Uid);
        return resource;
    }

    bool ResourceExists(const std::string& resourceName) const
    {
        return hResources.contains(resourceName);
    }
};