#include "Render/Drawable.h"
#include "Render/D3D11/D3D11Renderer.h"

namespace Render
{
    Drawable::Drawable()
    {
        m_hMesh = {};
        m_hTexture = {};
    }

    void Drawable::AttachMesh(const std::string& meshName)
    {
        m_hMesh = ResourceManager::Get().MeshLocator.GetResourceHandle(meshName);
    }

    void Drawable::AttachTexture(const std::string& textureName)
    {
        m_hTexture = ResourceManager::Get().TextureLocator.GetResourceHandle(textureName);
    }

    void Drawable::Draw() const
    {
        const auto& resourceManager = ResourceManager::Get();
        const Mesh& mesh = resourceManager.MeshLocator.LookupResource(m_hMesh);

        g_pDeviceContext->IASetVertexBuffers(mesh.VertexBufferSlot, mesh.VertexBufferCount, 
                                             &mesh.pVertexBuffer, &mesh.VertexBufferStride, 
                                             &mesh.VertexBufferOffset);
        // TODO: This should be part of the mesh.
        g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        auto& entityManager = ECS::EntityManager::Get();
        if (entityManager.HasComponent<Transformable>(Entity))
        {
            entityManager.GetComponent<Transformable>(Entity).OnRender();
        }

        if (m_hTexture.IsValid())
        {
            const auto& texture = resourceManager.TextureLocator.LookupResource(m_hTexture);
            g_pDeviceContext->PSSetShaderResources(0, 1, &texture.pTextureView);
        }

        g_pDeviceContext->Draw(mesh.VertexCount, mesh.StartVertexLocation);
    }
}