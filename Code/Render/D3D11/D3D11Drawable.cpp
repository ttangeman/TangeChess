#include "Render/Drawable.h"
#include "Render/D3D11/D3D11Renderer.h"

namespace Tange
{
    Drawable::Drawable()
    {
        m_pColorBuffer = std::make_shared<GpuBuffer>(sizeof(Vec4));
    }
    
    Drawable::Drawable(Drawable&& other)
    {
        std::memcpy(this, &other, sizeof(*this));
        std::memset(&other, 0, sizeof(*this));
    }

    void Drawable::AttachMesh(const std::string& meshName)
    {
        m_hMesh = ResourceManager::MeshLocator.GetResourceHandle(meshName);
    }

    void Drawable::AttachTexture(const std::string& textureName)
    {
        m_hTexture = ResourceManager::TextureLocator.GetResourceHandle(textureName);
    }

    void Drawable::SetColor(Vec4 color)
    {
        auto* buffer = static_cast<Vec4*>(m_pColorBuffer->MapBuffer());
        buffer[0] = color;
        m_pColorBuffer->Unmap();
    }

    void Drawable::OnRender()
    {
        const Mesh& mesh = ResourceManager::MeshLocator.LookupResource(m_hMesh);

        g_pDeviceContext->IASetVertexBuffers(mesh.VertexBufferSlot, mesh.VertexBufferCount, 
                                             &mesh.pVertexBuffer, &mesh.VertexBufferStride, 
                                             &mesh.VertexBufferOffset);
        // TODO: This should be part of the mesh.
        g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        m_pColorBuffer->SetBuffer(BufferBinding::PixelShader, 0);

        if (m_hTexture.IsValid())
        {
            const auto& texture = ResourceManager::TextureLocator.LookupResource(m_hTexture);
            g_pDeviceContext->PSSetShaderResources(0, 1, &texture.pTextureView);
        }

        g_pDeviceContext->Draw(mesh.VertexCount, mesh.StartVertexLocation);
    }
}