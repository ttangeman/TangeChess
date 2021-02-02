#pragma once

#include "Render/Renderer.h"
#include "Entity/Component.h"
// TODO: This should not be here, but I think there's a circular
// dependency somewhere.
#include "Render/D3D11/D3D11Renderer.h"

namespace Tange
{
    class Transformable : public Component<Transformable>
    {
        Transform m_transform;
        
    public:
        // TODO: Switch to 3D.
        Vec2 Position;
        Vec3 Orientation;
        Vec3 Scale;

        Transformable();
        Transformable(Transformable&& other);
        Transformable(const Transformable&) = default;

        // Changes to orthographic mode.
        void SetOrthographic(Vec2 minView, Vec2 maxView, float nearZ, float farZ);
        
        // Updates the GPU buffer with the transformed matrices.
        void OnUpdate();

        void OnRender();

        void SetTransformBuffer(uint32 slot);
    };
}