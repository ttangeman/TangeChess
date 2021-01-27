#pragma once

#include "Core/Common.h"
#include "Render/RenderObject.h"

namespace World
{
    using Render::RenderObject;
    
    class Entity
    {
        public:

        int32 Id;
        Vec3 Position;
        Vec3 Orientation;
        Vec3 Scale;

        virtual void OnUpdate()
        {
        }

        virtual void OnRender()
        {

        }

        private:

        Handle<RenderObject> m_renderObject;
    };
}