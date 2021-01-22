#include "Render/ResourceManager.h"
#include "Render/D3D11/D3D11Renderer.h"

namespace Render
{
    ResourceManager& ResourceManager::GetInstance()
    {
        static ResourceManager instance;
        return instance;
    }
    
    ResourceManager::ResourceManager()
    {
        // One might expect a lot of meshes, textures, and shaders to be submitted,
        // so it is worth reserving space for them.
        m_meshes.reserve(64);
        m_shaders.reserve(8);
        m_textures.reserve(32);
    }

    ResourceManager::~ResourceManager()
    {
        for (auto& mesh : m_meshes)
        {
            SafeRelease(mesh.pVertexBuffer);
            SafeRelease(mesh.pIndexBuffer);
        }

        for (auto& shader : m_shaders)
        {
            SafeRelease(shader.pVertexShader);
            SafeRelease(shader.pPixelShader);
            SafeRelease(shader.pInputLayout);
        }

        for (auto& texture : m_textures)
        {
            SafeRelease(texture.pTextureView);
        }
    }
    
    void ResourceManager::SubmitMesh(const std::string& meshName, const void* pVertexData, 
                                     usize vertexCount, usize vertexSize)
    {
        // Create the vertex buffer.
        ID3D11Buffer* pVertexBuffer;
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = vertexCount * vertexSize;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        // TODO: Configuration for these.
        //bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        //bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        
        D3D11_SUBRESOURCE_DATA data = {pVertexData, 0, 0};
        
        HRESULT result = g_pDevice->CreateBuffer(&bufferDesc, &data, &pVertexBuffer);
        CheckResult(result);
        
        Mesh mesh = {};
        mesh.Id = m_meshAccumulator++;
        mesh.pVertexBuffer = pVertexBuffer;
        mesh.VertexBufferCount = 1;
        mesh.VertexBufferStride = vertexSize;
        mesh.VertexCount = vertexCount;

        m_meshes.emplace_back(mesh);
        m_hMeshTable.emplace(
            std::make_pair(meshName, Handle<Mesh>(mesh.Id, m_meshes.size() - 1))
        );
    }
    
    Handle<Mesh> ResourceManager::GetMeshHandle(const std::string& meshName) const
    {
        return m_hMeshTable.at(meshName);
    }
    
    const Mesh& ResourceManager::LookupMesh(Handle<Mesh> hMesh) const
    {
        const auto& mesh = m_meshes.at(hMesh.GetIndex());
        // Assert that we are not referencing a stale handle.
        Assert(mesh.Id == hMesh.GetId());
        return mesh;
    }

    Mesh& ResourceManager::GetMesh(Handle<Mesh> hMesh)
    {
        auto& mesh = m_meshes.at(hMesh.GetIndex());
        // Assert that we are not referencing a stale handle.
        Assert(mesh.Id == hMesh.GetId());
        return mesh;
    }
    
    bool ResourceManager::MeshExists(const std::string& meshName) const
    {
        // C++20 has a .contains() but my C/C++ extension in VSCode can't find it.
        // (Even though it compiles). 
        //return m_hShaderTable.contains(shaderName);
        
        // Less elegant alternative.
        auto query = m_hMeshTable.find(meshName);
        return (query != m_hMeshTable.end()) ? true : false;
    }

    void ResourceManager::SubmitShader(const std::string& shaderName,
                                       const void* pVertexShaderData, usize vertexShaderSize, 
                                       const void* pPixelShaderData, usize pixelShaderSize)

    {
        Shader shader = {};
        
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = 
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };       

        HRESULT result = g_pDevice->CreateVertexShader(pVertexShaderData, vertexShaderSize, 
                                                       nullptr, &shader.pVertexShader);
        CheckResult(result);

        // TODO: Probably don't need to create this every time...
        result = g_pDevice->CreateInputLayout(inputElementDesc, ArrayLength(inputElementDesc),
                                              pVertexShaderData, vertexShaderSize,
                                              &shader.pInputLayout);
        CheckResult(result);

        result = g_pDevice->CreatePixelShader(pPixelShaderData, pixelShaderSize, 
                                              nullptr, &shader.pPixelShader);
        CheckResult(result);

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

    const Shader& ResourceManager::LookupShader(Handle<Shader> hShader) const
    {
        const auto& shader = m_shaders.at(hShader.GetIndex());
        // Assert that we are not referencing a stale handle.
        Assert(shader.Id == hShader.GetId());
        return shader;
    }

    Shader& ResourceManager::GetShader(Handle<Shader> hShader)
    {
        auto& shader = m_shaders.at(hShader.GetIndex());
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

    const Texture& ResourceManager::LookupTexture(Handle<Texture> hTexture) const
    {
        const auto& texture = m_textures.at(hTexture.GetIndex());
        // Assert that we are not referencing a stale handle.
        Assert(texture.Id == hTexture.GetId());
        return texture;
    }

    Texture& ResourceManager::GetTexture(Handle<Texture> hTexture)
    {
        auto& texture = m_textures.at(hTexture.GetIndex());
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