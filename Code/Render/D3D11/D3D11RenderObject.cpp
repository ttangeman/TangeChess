#include "Render/RenderObject.h"
#include "Render/D3D11/D3D11Renderer.h"

namespace Render
{
    RenderObject::RenderObject()
    {
        m_hMesh = {};
        m_hTexture = {};
        m_pTransform = std::make_unique<Transform>(Render::CreateTransform());
    }

    void RenderObject::AttachMesh(const std::string& meshName)
    {
        m_hMesh = ResourceManager::GetInstance().MeshLocator.GetResourceHandle(meshName);
    }

    void RenderObject::AttachTexture(const std::string& textureName)
    {
        m_hTexture = ResourceManager::GetInstance().TextureLocator.GetResourceHandle(textureName);
    }

    void RenderObject::SetOrthographic(Vec2 minView, Vec2 maxView, float nearZ, float farZ)
    {
        XMMATRIX orthographic = DirectX::XMMatrixOrthographicOffCenterLH(minView.X, maxView.X,
                                                                         minView.Y, maxView.Y,
                                                                         nearZ, farZ);
        m_pTransform->Projection = orthographic;
    }

    void RenderObject::Update(Vec3 position, Vec3 scaling, Vec3 rotation)
    {
        XMMATRIX scale = DirectX::XMMatrixScaling(scaling.X, scaling.Y, scaling.Z);
        XMMATRIX rotationX = DirectX::XMMatrixRotationX(DEGREES_TO_RADIANS(rotation.X));
        XMMATRIX rotationY = DirectX::XMMatrixRotationY(DEGREES_TO_RADIANS(rotation.Y));
        XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(DEGREES_TO_RADIANS(rotation.Z));
        XMMATRIX translation = DirectX::XMMatrixTranslation(position.X, position.Y, position.Z);

        // NOTE: Order is super important! Rotations should come before translation!
        XMMATRIX affineTransform = scale * rotationX * rotationY * rotationZ * translation;

        D3D11_MAPPED_SUBRESOURCE data;
        HRESULT result = g_pDeviceContext->Map(m_pTransform->pTransformBuffer, 0, 
                                               D3D11_MAP_WRITE_DISCARD, 0, &data);
        CHECK_RESULT(result);
        
        TransformData* pTransformData = (TransformData*)data.pData;
        pTransformData->World = DirectX::XMMatrixTranspose(affineTransform);
        pTransformData->View = DirectX::XMMatrixIdentity();
        pTransformData->Projection = DirectX::XMMatrixTranspose(m_pTransform->Projection);
        
        g_pDeviceContext->Unmap(m_pTransform->pTransformBuffer, 0);
    }

    void RenderObject::Draw() const
    {
        const auto& resourceManager = ResourceManager::GetInstance();
        const Mesh& mesh = resourceManager.MeshLocator.LookupResource(m_hMesh);

        g_pDeviceContext->IASetVertexBuffers(mesh.VertexBufferSlot, mesh.VertexBufferCount, 
                                             &mesh.pVertexBuffer, &mesh.VertexBufferStride, 
                                             &mesh.VertexBufferOffset);
        // TODO: This should be parameterized.
        g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        g_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pTransform->pTransformBuffer);

        if (m_hTexture.IsValid())
        {
            const auto& texture = ResourceManager::GetInstance().TextureLocator.LookupResource(m_hTexture);
            g_pDeviceContext->PSSetShaderResources(0, 1, &texture.pTextureView);
        }

        g_pDeviceContext->Draw(mesh.VertexCount, mesh.StartVertexLocation);
    }
}