#include "Render/RenderObject.h"
#include "Render/D3D11/D3D11Renderer.h"

namespace Render
{
    void RenderObject::AttachMesh(const std::string& meshName)
    {
        m_hMesh = ResourceManager::GetInstance().MeshLocator.GetElementHandle(meshName);
    }

    void RenderObject::AttachTexture(const std::string& textureName)
    {
        m_hTexture = ResourceManager::GetInstance().TextureLocator.GetElementHandle(textureName);
    }

    void RenderObject::Draw() const
    {
        const auto& resourceManager = ResourceManager::GetInstance();
        const Mesh& mesh = resourceManager.MeshLocator.LookupElement(m_hMesh);

        g_pDeviceContext->IASetVertexBuffers(mesh.VertexBufferSlot, mesh.VertexBufferCount, 
                                             &mesh.pVertexBuffer, &mesh.VertexBufferStride, 
                                             &mesh.VertexBufferOffset);
        // TODO: This should be parameterized.
        g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        //g_pDeviceContext->VSSetConstantBuffers(0, 1, _transforms_);

        if (m_hTexture.IsValid())
        {
        //g_pDeviceContext->PSSetShaderResources(0, 1, &texture.pTextureView);
        }

        g_pDeviceContext->Draw(mesh.VertexCount, mesh.StartVertexLocation);
    }
}