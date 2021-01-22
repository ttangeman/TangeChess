#include "Render/ResourceManager.h"
#include "Render/D3D11/D3D11Renderer.h"

namespace Render
{
    ResourceManager& ResourceManager::GetInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    ResourceManager::~ResourceManager()
    {
        for (auto& mesh : MeshLocator.Elements)
        {
            SAFE_RELEASE(mesh.pVertexBuffer);
            SAFE_RELEASE(mesh.pIndexBuffer);
        }

        for (auto& shader : ShaderLocator.Elements)
        {
            SAFE_RELEASE(shader.pVertexShader);
            SAFE_RELEASE(shader.pPixelShader);
            SAFE_RELEASE(shader.pInputLayout);
        }

        for (auto& texture : TextureLocator.Elements)
        {
            SAFE_RELEASE(texture.pTextureView);
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
        CHECK_RESULT(result);
        
        Mesh mesh = {};
        mesh.pVertexBuffer = pVertexBuffer;
        mesh.VertexBufferCount = 1;
        mesh.VertexBufferStride = vertexSize;
        mesh.VertexCount = vertexCount;

        MeshLocator.PushElement(meshName, &mesh);
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
        CHECK_RESULT(result);

        // TODO: Probably don't need to create this every time...
        result = g_pDevice->CreateInputLayout(inputElementDesc, ARRAY_LENGTH(inputElementDesc),
                                              pVertexShaderData, vertexShaderSize,
                                              &shader.pInputLayout);
        CHECK_RESULT(result);

        result = g_pDevice->CreatePixelShader(pPixelShaderData, pixelShaderSize, 
                                              nullptr, &shader.pPixelShader);
        CHECK_RESULT(result);

        ShaderLocator.PushElement(shaderName, &shader);
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
        ASSERT(bytesPerPixel == 4 || bytesPerPixel == 1);
        textureDesc.Format = (bytesPerPixel == 4) ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        //textureDesc.Usage = D3D11_USAGE_DYNAMIC;
        //textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
        
        // Load the texture to the GPU.
        ID3D11Texture2D* pTexture;
        HRESULT result = g_pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
        CHECK_RESULT(result);
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
        CHECK_RESULT(result);
        g_pDeviceContext->GenerateMips(pTextureView);
        
        Texture texture;
        texture.pTextureView = pTextureView;

        TextureLocator.PushElement(textureName, &texture);
    }
}