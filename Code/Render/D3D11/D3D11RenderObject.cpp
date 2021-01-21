#include "Render/RenderObject.h"
#include "Render/D3D11/D3D11Renderer.h"

namespace Render
{
    void RenderObject::AttachMesh(const std::string& meshName)
    {
        const auto& resourceManager = ResourceManager::GetInstance();
        m_hMesh = resourceManager.GetMeshHandle(meshName);
    }

    void RenderObject::AttachTexture(const std::string& textureName)
    {
        const auto& resourceManager = ResourceManager::GetInstance();
        m_hTexture = resourceManager.GetTextureHandle(textureName);
    }
}