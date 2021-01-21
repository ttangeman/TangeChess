#include "Render/D3D11/D3D11RenderObject.h"

namespace Render
{
    void RenderObject::AttachShader(const std::string& shaderName)
    {
        const auto& resourceManager = ResourceManager::GetInstance();
        m_hShader = resourceManager.GetShaderHandle(shaderName);
    }
    
    void RenderObject::AttachTexture(const std::string& textureName)
    {
        const auto& resourceManager = ResourceManager::GetInstance();
        m_hTexture = resourceManager.GetTextureHandle(textureName);
    }
}