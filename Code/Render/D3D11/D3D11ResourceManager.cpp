#include "D3D11ResourceManager.h"

namespace Render
{
    Shader::~Shader()
    {
        SafeRelease(pVertexShader);
        SafeRelease(pPixelShader);
        SafeRelease(pInputLayout);
    }

    ShaderHandle::ShaderHandle(const std::vector<Shader>& shaderTable, uint32 uid, usize index) 
        : m_uid(uid), m_index(index), m_shaderTable(shaderTable) 
    {
        Assert(uid != 0);
    }
    
    void ShaderHandle::SetIndex(usize index)
    {
        m_index = index;
    }
    
    const Shader& ShaderHandle::Get() const
    {
        const auto& shader = m_shaderTable.at(m_index);
        // Assert that we are not referencing a stale handle.
        Assert(shader.Id == m_uid);
        return shader;
    }

    Texture::~Texture()
    {
        SafeRelease(pTextureView);
    }

    TextureHandle::TextureHandle(const std::vector<Texture>& textureTable, uint32 uid, usize index) 
        : m_uid(uid), m_index(index), m_textureTable(textureTable) 
    {
        Assert(uid != 0);
    }
    
    void TextureHandle::SetIndex(usize index)
    {
        m_index = index;
    }
    
    const Texture& TextureHandle::Get() const
    {
        const auto& texture = m_textureTable.at(m_index);
        // Assert that we are not referencing a stale handle.
        Assert(texture.Id == m_uid);
        return texture;
    }

    D3D11ResourceManager::D3D11ResourceManager()
        : m_shaderUidAccumulator(0), m_textureUidAccumulator(0)
    {
        // One might expect a lot of textures and shaders to be submitted.
        // So it is worth reserving space for them.
        m_shaders.reserve(16);
        m_textures.reserve(32);

        // The uid's for handles cannot be 0.
        m_shaderUidAccumulator = 1;
        m_textureUidAccumulator = 1;
    }
    
    ShaderHandle D3D11ResourceManager::SubmitShader(const std::string& vertexShaderByteCode, 
                                                    const std::string& pixelShaderByteCode)
    {
        Shader shader = {};
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = 
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };       

        g_pDevice->CreateVertexShader(vertexShaderByteCode.c_str(), 
                                      vertexShaderByteCode.length(), 
                                      nullptr, &shader.pVertexShader);

        g_pDevice->CreateInputLayout(inputElementDesc, ArrayLength(inputElementDesc),
                                     vertexShaderByteCode.c_str(), vertexShaderByteCode.length(),
                                     &shader.pInputLayout);

        g_pDevice->CreatePixelShader(pixelShaderByteCode.c_str(), 
                                     pixelShaderByteCode.length(), 
                                     nullptr, &shader.pPixelShader);
        
        shader.Id = m_shaderUidAccumulator++;
        m_shaders.push_back(shader);
        
        return ShaderHandle(m_shaders, shader.Id, m_shaders.size() - 1);
    }
    
    TextureHandle D3D11ResourceManager::SubmitTexture()
    {
        return TextureHandle(m_textures, 1337, 1337);
    }
}