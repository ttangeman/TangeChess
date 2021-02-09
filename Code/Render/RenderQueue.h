#pragma once

#include "Core/Common.h"
#include "Render/Renderer.h"
#include "Render/RenderHandle.h"

namespace Tange
{   
    // NOTE: This should be implemented for each render API.
    // In the future, this might construct scenes/render graphs,
    // but for the time being it just submits stuff to the GPU --
    // plain and simple.
    class RenderQueue
    {
    public:
        // TODO: In the future we can have a hash table or something
        // that lets us queue up all renderables under a shader name, so
        // that it does not get set more than needed.
        void Submit(const std::string& shaderName, 
                    const RenderHandle& hRender, 
                    const Transform& transform) const;
    };
}