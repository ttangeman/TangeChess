#pragma once

#include "Core/Common.h"
#include "Utility/Handle.h"

// Maps names to handles which map to data. It is dynamic because it uses a std::vector.
template <typename T>
class DynamicHandleLocator
{
    public:

    std::vector<T> Elements;
    std::unordered_map<std::string, Handle<T>> hElements;

    DynamicHandleLocator(usize reserveSize = 8)
    {
        Elements.reserve(reserveSize);
        hElements.reserve(reserveSize);
    }

    // NOTE: T _must_ have an Id field which is modified by this function!
    // Pushes the element into the vector and maps a handle to the data
    // under the element name.
    void PushElement(const std::string& elementName, T* element)
    {
        element->Id = m_accumulator++;
        Elements.emplace_back(*element);
        hElements.emplace(
            std::make_pair(elementName, Handle<T>(element->Id, Elements.size() - 1))
        );
    }

    Handle<T> GetElementHandle(const std::string& elementName) const
    {
        return hElements.at(elementName);
    }

    const T& LookupElement(Handle<T> hElement) const
    {
        const T& element = Elements.at(hElement.GetIndex());
        // Assert that we are not referencing a stale handle.
        ASSERT(element.Id == hElement.GetId());
        return element;
    }

    T& GetElement(Handle<T> hElement)
    {
        T& element = Elements.at(hElement.GetIndex());
        // Assert that we are not referencing a stale handle.
        ASSERT(element.Id == hElement.GetId());
        return element;
    }

    bool ElementExists(const std::string& elementName) const
    {
        return hElements.contains(elementName);
    }

    private:

    // Handle unique id's cannot be 0.
    uint32 m_accumulator = 1;
};