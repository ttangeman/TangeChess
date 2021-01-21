#include "D3D11ResourceManager.h"

namespace Render
{
    Shader::~Shader()
    {
        SafeRelease(pVertexShader);
        SafeRelease(pPixelShader);
        SafeRelease(pInputLayout);
    }

    Texture::Texture(uint64 id, ID3D11ShaderResourceView* pTextureView)
        : Id(id), pTextureView(pTextureView)
    {
    }
    
    Texture::~Texture()
    {
        SafeRelease(pTextureView);
    }

    ResourceManager& ResourceManager::GetInstance()
    {
        static ResourceManager instance;
        return instance;
    }
    
    ResourceManager::ResourceManager()
        // The uid's for handles cannot be 0.
        : m_shaderAccumulator(1), m_textureAccumulator(1)
    {
        // One might expect a lot of textures and shaders to be submitted.
        // So it is worth reserving space for them.
        m_shaders.reserve(8);
        m_textures.reserve(32);
    }
    
   void ResourceManager::SubmitShader(const std::string& shaderName,
                                      const std::string& vertexShaderByteCode, 
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

        // TODO: Probably don't need to create this every time...
        g_pDevice->CreateInputLayout(inputElementDesc, ArrayLength(inputElementDesc),
                                     vertexShaderByteCode.c_str(), vertexShaderByteCode.length(),
                                     &shader.pInputLayout);

        g_pDevice->CreatePixelShader(pixelShaderByteCode.c_str(), 
                                     pixelShaderByteCode.length(), 
                                     nullptr, &shader.pPixelShader);

        shader.Id = m_shaderAccumulator++;
        m_shaders.emplace_back(shader);
        m_hShaderTable.emplace(
            std::make_pair(shaderName, Handle<Shader>(shader.Id, m_shaders.size() - 1))
        );

    }
    
    Handle<Shader> ResourceManager::GetShaderHandle(const std::string& shaderName) const
    {
        return m_hShaderTable.at(shaderName);
    }

    const Shader& ResourceManager::GetShader(Handle<Shader> hShader) const
    {
        const auto& shader = m_shaders.at(hShader.GetIndex());
        // Assert that we are not referencing a stale handle.
        Assert(shader.Id == hShader.GetId());
        return shader;
    }
    
    bool ResourceManager::ShaderExists(const std::string& shaderName) const
    {
        // C++20 has a .contains() but my C/C++ extension in VSCode can't find it.
        // (Even though it compiles). 
        //return m_hShaderTable.contains(shaderName);
        
        // Less elegant alternative.
        auto query = m_hShaderTable.find(shaderName);
        return (query != m_hShaderTable.end()) ? true : false;
    }
    
    // TODO: Smarter texture recognition/texture atlas parameters.
    void ResourceManager::SubmitTexture(const std::string& textureName, 
                                        const void* pPixels, 
                                        int32 width, int32 height, int32 pitch, 
                                        int32 bytesPerPixel)
    {
        // Describe the texture.
        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.ArraySize = 1;
        // NOTE: Only standard RGBA bitmaps and Monochrome bitmaps are supported.
        Assert(bytesPerPixel == 4 || bytesPerPixel == 1);
        textureDesc.Format = (bytesPerPixel == 4) ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        //textureDesc.Usage = D3D11_USAGE_DYNAMIC;
        //textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
        
        // Load the texture to the GPU.
        ID3D11Texture2D* pTexture;
        HRESULT result = g_pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
        CheckResult(result);
        g_pDeviceContext->UpdateSubresource(pTexture, 0, nullptr, pPixels, pitch, 0);
        
        // Describe shader resource for the texture view.
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = -1;
        
        // Create the texture view.
        ID3D11ShaderResourceView* pTextureView;
        result = g_pDevice->CreateShaderResourceView(pTexture, &srvDesc, &pTextureView);
        CheckResult(result);
        g_pDeviceContext->GenerateMips(pTextureView);
        
        m_textures.emplace_back(Texture(m_textureAccumulator, pTextureView));
        m_hTextureTable.emplace(
            std::make_pair(textureName, Handle<Texture>(m_textureAccumulator, m_textures.size() - 1))
        );
        m_textureAccumulator++;
    }

    Handle<Texture> ResourceManager::GetTextureHandle(const std::string& textureName) const
    {
        return m_hTextureTable.at(textureName);
    }

    const Texture& ResourceManager::GetTexture(Handle<Texture> hTexture) const
    {
        const auto& texture = m_textures.at(hTexture.GetIndex());
        // Assert that we are not referencing a stale handle.
        Assert(texture.Id == hTexture.GetId());
        return texture;
    }

    bool ResourceManager::TextureExists(const std::string& textureName) const
    {
        // C++20 has a .contains() but my C/C++ extension in VSCode can't find it.
        // (Even though it compiles). 
        //return m_hTextureTable.contains(textureName);
        
        // Less elegant alternative.
        auto query = m_hTextureTable.find(textureName);
        return (query != m_hTextureTable.end()) ? true : false;
    }
}