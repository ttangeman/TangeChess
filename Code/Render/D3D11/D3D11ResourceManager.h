#pragma once

#include "Utility/Handle.h"
#include "Render/ResourceManager.h"
#include "Render/D3D11/D3D11Renderer.h"

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

namespace Render
{
    using namespace DirectX;

    struct Shader
    {
        uint64 Id;
        ID3D11VertexShader* pVertexShader;
        ID3D11PixelShader* pPixelShader;
        ID3D11InputLayout* pInputLayout;

        ~Shader(); 
    };
    
    struct Texture
    {
        uint64 Id;
        ID3D11ShaderResourceView* pTextureView;
        
        Texture(uint64 id, ID3D11ShaderResourceView* pTextureView);
        ~Texture();
    };
    
    class ResourceManager
    {
        public:

        static ResourceManager& GetInstance();
        ResourceManager();
        ResourceManager operator=(const ResourceManager&) = delete;
        ResourceManager(const ResourceManager&) = delete;

        // TODO: SetShader()?
        // m_activeShader?
        // this way we dont set the shader when we draw every time?

        // Registers a pre-compiled shader with D3D11 under the provided name.
        void SubmitShader(const std::string& shaderName,
                          const std::string& vertexShaderCode, 
                          const std::string& pixelShaderCode);

        // Retrieves the texture handle via the texture name.
        Handle<Shader> GetShaderHandle(const std::string& shaderName) const;

        // Retrieves the shader via the handle.
        const Shader& GetShader(Handle<Shader> hShader) const;
        
        bool ShaderExists(const std::string& shaderName) const;
        
        // Submits an image to the GPU as a texture under the provided name.
        void SubmitTexture(const std::string& textureName, const void* pPixels, 
                           int32 width, int32 height, int32 pitch, int32 bytesPerPixel);
        
        // Retrieves the texture handle via the texture name.
        Handle<Texture> GetTextureHandle(const std::string& textureName) const;

        // Retrieves the texture via the handle.
        const Texture& GetTexture(Handle<Texture> hTexture) const;
        
        bool TextureExists(const std::string& textureName) const;

        private:

        uint64 m_shaderAccumulator;
        std::vector<Shader> m_shaders;
        std::unordered_map<std::string, Handle<Shader>> m_hShaderTable;

        uint64 m_textureAccumulator;
        std::vector<Texture> m_textures;
        std::unordered_map<std::string, Handle<Texture>> m_hTextureTable;
    };
}