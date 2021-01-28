#pragma once

#include "Render/Renderer.h"
#include "World/IComponent.h"
#include "Render/D3D11/D3D11Renderer.h"

namespace Render
{
    class Transformable : public World::IComponent
    {
        public:
        // TODO: Switch to 3D.
        Vec2 Position;
        Vec3 Orientation;
        Vec3 Scale;

        Transformable();

        // Changes to orthographic mode.
        void SetOrthographic(Vec2 minView, Vec2 maxView, float nearZ, float farZ);
        
        // Updates the GPU buffer with the transformed matrices.
        void OnUpdate();

        void OnRender() const;

        private:

        Transform m_transform;
    };
}