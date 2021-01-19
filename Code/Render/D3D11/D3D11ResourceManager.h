#pragma once

#include "Render/Renderer.h"
#include "Render/IResourceManager.h"
#include "Render/D3D11/D3D11Renderer.h"

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

namespace Render
{
    using namespace DirectX;

    struct Vertex
    {
        Vec3 Position;
        Vec4 Color;
        Vec2 TexCoord;
    };
    
    struct Shader
    {
        uint32 Id;
        ID3D11VertexShader* pVertexShader;
        ID3D11PixelShader* pPixelShader;
        ID3D11InputLayout* pInputLayout;

        ~Shader(); 
    };
    
    class ShaderHandle
    {
        public:

        ShaderHandle(const std::vector<Shader>& shaderTable, uint32 uid, usize index);
        
        // Changes the index of the uniquely identified shader.
        void SetIndex(usize index);
        
        // Gets the shader from the table.
        const Shader& Get() const;
        
        private:

        // NOTE: A uid of 0 is considered an invalid shader handle.
        // Uniqely identifies the shader.
        uint32 m_uid;
        
        // Indexes the shader table
        usize m_index;
        
        // Stores the actual shader.
        const std::vector<Shader>& m_shaderTable;
        
        ShaderHandle();
    };
    
    struct Texture
    {
        uint32 Id;
        ID3D11ShaderResourceView* pTextureView;
        
        ~Texture();
    };

    class TextureHandle
    {
        public:

        TextureHandle(const std::vector<Texture>& textureTable, uint32 uid, usize index);
        
        // Changes the index of uniquely identified texture.
        void SetIndex(usize index);
        
        // Gets the texture from the table.
        const Texture& Get() const;
        
        private:

        // NOTE: A uid of 0 is considered an invalid texture handle.
        // Uniqely identifies the texture.
        uint32 m_uid;
        
        // Indexes the texture table.
        usize m_index;
        
        // Stores the actual texture.
        const std::vector<Texture>& m_textureTable;
        
        TextureHandle();
    };
    
    class RenderObject
    {
        public:

        private:

        TextureHandle m_textureHandle;
        ID3D11Buffer* m_pVertexBuffer;
        ID3D11Buffer* m_pTransform;
        XMMATRIX m_projection;
    };

    class D3D11ResourceManager : public IResourceManager
    {
        public:

        D3D11ResourceManager();

        // Registers a pre-compiled shader with D3D11.
        ShaderHandle SubmitShader(const std::string& vertexShaderByteCode, 
                                  const std::string& pixelShaderByteCode);
        
        // TODO: SetShader()?
        // m_activeShader?
        // this way we dont set the shader when we draw every time?

        // Submits an image to the GPU as a texture.
        TextureHandle SubmitTexture();

        private:

        // TODO: A vector is not a scalable data structrue for a more advanced
        // renderer. But, it is the simplest data structure to implement for a 
        // small renderer like this one. A hash map would likely be a better solution.

        std::vector<Shader> m_shaders;
        int32 m_shaderUidAccumulator;

        std::vector<Texture> m_textures;
        int32 m_textureUidAccumulator;
    };
}