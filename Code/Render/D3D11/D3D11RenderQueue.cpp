#include "Core/Common.h"
#include "Render/RenderQueue.h"

namespace Tange
{
    void RenderQueue::Submit(const std::string& shaderName, 
                             const RenderHandle& hRender, 
                             const Transform& transform) const
    {
        Renderer::SetShader(shaderName);
        
        const Mesh& mesh = hRender.GetMesh();
        g_pDeviceContext->IASetVertexBuffers(mesh.VertexBufferSlot, mesh.VertexBufferCount, 
                                             &mesh.pVertexBuffer, &mesh.VertexBufferStride, 
                                             &mesh.VertexBufferOffset);
        // TODO: This should be part of the mesh.
        g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        transform.pTransformBuffer->SetBuffer(BufferBinding::VertexShader, 0);
        hRender.pColorBuffer->SetBuffer(BufferBinding::PixelShader, 0);

        if (hRender.hTexture.IsValid())
        {
            const Texture& texture = hRender.GetTexture();
            g_pDeviceContext->PSSetShaderResources(0, 1, &texture.pTextureView);
        }
        
        g_pDeviceContext->Draw(mesh.VertexCount, mesh.StartVertexLocation);
    }
}