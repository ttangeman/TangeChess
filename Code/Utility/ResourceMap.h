#pragma once

#include "Core/Common.h"
#include "Utility/Handle.h"

// Maps names to handles which map to data. It is dynamic because it uses a std::vector.
template <typename T>
class ResourceMap
{
    public:

    std::vector<T> Resources;
    std::unordered_map<std::string, Handle<T>> hResources;

    ResourceMap(usize reserveSize = 8)
    {
        Resources.reserve(reserveSize);
        hResources.reserve(reserveSize);
    }

    // NOTE: T _must_ have an Id field which is modified by this function!
    // Pushes the element into the vector and maps a handle to the data
    // under the element name.
    void PushResource(const std::string& resourceName, T& resource)
    {
        resource.Id = m_accumulator++;
        Resources.emplace_back(resource);
        hResources.emplace(
            std::make_pair(resourceName, Handle<T>(resource.Id, Resources.size() - 1))
        );
    }

    Handle<T> GetResourceHandle(const std::string& resourceName) const
    {
        return hResources.at(resourceName);
    }

    const T& LookupResource(Handle<T> hResource) const
    {
        const T& resource = Resources.at(hResource.GetIndex());
        // Assert that we are not referencing a stale handle.
        ASSERT(resource.Id == hResource.GetId());
        return resource;
    }

    T& GetResource(Handle<T> hResource)
    {
        T& resource = Resources.at(hResource.GetIndex());
        // Assert that we are not referencing a stale handle.
        ASSERT(resource.Id == hResource.GetId());
        return resource;
    }

    bool ResourceExists(const std::string& resourceName) const
    {
        return hResources.contains(resourceName);
    }

    private:

    // Handle unique id's cannot be 0.
    uint32 m_accumulator = 1;
};