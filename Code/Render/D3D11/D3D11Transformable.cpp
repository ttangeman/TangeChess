#include "Render/Transformable.h"

namespace Tange
{
    Transformable::Transformable()
    {
        m_transform = CreateTransform();
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

        D3D11_MAPPED_SUBRESOURCE data;
        HRESULT result = g_pDeviceContext->Map(m_transform.pTransformBuffer, 0, 
                                               D3D11_MAP_WRITE_DISCARD, 0, &data);
        CHECK_RESULT(result);
        
        TransformData* pTransformData = (TransformData*)data.pData;
        pTransformData->World = DirectX::XMMatrixTranspose(affineTransform);
        // TODO: Camera
        pTransformData->View = DirectX::XMMatrixIdentity();
        pTransformData->Projection = DirectX::XMMatrixTranspose(m_transform.Projection);
        
        g_pDeviceContext->Unmap(m_transform.pTransformBuffer, 0);
    }

    void Transformable::OnRender() const
    {
        g_pDeviceContext->VSSetConstantBuffers(0, 1, &m_transform.pTransformBuffer);
    }
}