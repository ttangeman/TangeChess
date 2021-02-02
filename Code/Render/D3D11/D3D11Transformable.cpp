#include "Render/Transformable.h"

namespace Tange
{
    Transformable::Transformable()
    {
        m_transform.pTransformBuffer = std::make_shared<GpuBuffer>(sizeof(TransformData));
    }

    Transformable::Transformable(Transformable&& other)
    {
        std::memcpy(this, &other, sizeof(*this));
        std::memset(&other, 0, sizeof(*this));
    }

    void Transformable::SetOrthographic(Vec2 minView, Vec2 maxView, float nearZ, float farZ)
    {
        XMMATRIX orthographic = DirectX::XMMatrixOrthographicOffCenterLH(minView.X, maxView.X,
                                                                         minView.Y, maxView.Y,
                                                                         nearZ, farZ);
        m_transform.Projection = orthographic;
    }

    void Transformable::OnUpdate()
    {
        XMMATRIX scale = DirectX::XMMatrixScaling(Scale.X, Scale.Y, Scale.Z);
        XMMATRIX rotationX = DirectX::XMMatrixRotationX(DEGREES_TO_RADIANS(Orientation.X));
        XMMATRIX rotationY = DirectX::XMMatrixRotationY(DEGREES_TO_RADIANS(Orientation.Y));
        XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(DEGREES_TO_RADIANS(Orientation.Z));
        XMMATRIX translation = DirectX::XMMatrixTranslation(Position.X, Position.Y, 1);
        // TODO: Switch to 3D.
        ASSERT(typeid(Position) == typeid(Vec2));

        // NOTE: Order is super important! Rotations should come before translation!
        XMMATRIX affineTransform = scale * rotationX * rotationY * rotationZ * translation;

        auto* pTransformData = static_cast<TransformData*>(m_transform.pTransformBuffer->MapBuffer());

        pTransformData->World = DirectX::XMMatrixTranspose(affineTransform);
        // TODO: Camera
        pTransformData->View = DirectX::XMMatrixIdentity();
        pTransformData->Projection = DirectX::XMMatrixTranspose(m_transform.Projection);
        
        m_transform.pTransformBuffer->Unmap();
    }

    void Transformable::OnRender()
    {
        // TODO: Maybe have a way to query the renderer for which
        // slot the buffer should set.
        SetTransformBuffer(0);
    }

    void Transformable::SetTransformBuffer(uint32 slot)
    {
        m_transform.pTransformBuffer->SetBuffer(BufferBinding::VertexShader, slot);
    }
}