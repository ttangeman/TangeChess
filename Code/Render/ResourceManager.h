#pragma once

#include "Utility/Handle.h"
#include "Render/Renderer.h"

namespace Render
{
    class ResourceManager
    {
        public:

        static ResourceManager& GetInstance();
        ResourceManager();
        ~ResourceManager();
        ResourceManager operator=(const ResourceManager&) = delete;
        ResourceManager(const ResourceManager&) = delete;

        // Registers a mesh with the GPU under the provided name.
        void SubmitMesh(const std::string& meshName, const void* pVertexData, 
                        usize vertexCount, usize vertexSize);
        
        // Retrieves the mesh via the mesh name.
        Handle<Mesh> GetMeshHandle(const std::string& meshName) const;
        
        // Retrieves the mesh via the handle.
        const Mesh& LookupMesh(Handle<Mesh> hMesh) const;
        Mesh& GetMesh(Handle<Mesh> hMesh);
        
        bool MeshExists(const std::string& meshName) const;

        // Registers a pre-compiled shader under the provided name.
        void SubmitShader(const std::string& shaderName,
                          const void* pVertexShaderData, usize vertexShaderSize, 
                          const void* pPixelShaderData, usize pixelShaderSize);

        // Retrieves the texture handle via the texture name.
        Handle<Shader> GetShaderHandle(const std::string& shaderName) const;

        // Retrieves the shader via the handle.
        const Shader& LookupShader(Handle<Shader> hShader) const;
        Shader& GetShader(Handle<Shader> hShader);
        
        bool ShaderExists(const std::string& shaderName) const;
        
        // Submits an image to the GPU as a texture under the provided name.
        void SubmitTexture(const std::string& textureName, const void* pPixels, 
                           int32 width, int32 height, int32 pitch, int32 bytesPerPixel);
        
        // Retrieves the texture handle via the texture name.
        Handle<Texture> GetTextureHandle(const std::string& textureName) const;

        // Retrieves the texture via the handle.
        const Texture& LookupTexture(Handle<Texture> hTexture) const;
        Texture& GetTexture(Handle<Texture> hTexture);
        
        bool TextureExists(const std::string& textureName) const;

        private:
        
        // Each resource has an accumulator for assigning unique identifiers to the resource
        // and a vector of the POD (plain old data) alongside a hash table mapping unique names
        // to each of the resource handles. This scheme allows for robust memory safety when accessing
        // GPU resources.

        // NOTE: The uid's for handles cannot be 0.
        uint32 m_meshAccumulator = 1;
        std::vector<Mesh> m_meshes;
        std::unordered_map<std::string, Handle<Mesh>> m_hMeshTable;

        uint32 m_shaderAccumulator = 1;
        std::vector<Shader> m_shaders;
        std::unordered_map<std::string, Handle<Shader>> m_hShaderTable;

        uint32 m_textureAccumulator = 1;
        std::vector<Texture> m_textures;
        std::unordered_map<std::string, Handle<Texture>> m_hTextureTable;
    };
}