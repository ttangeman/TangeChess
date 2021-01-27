#pragma once

#include "Asset/Image.h"
#include "Utility/HandleMap.h"
#include "Render/Renderer.h"

namespace Render
{
    class ResourceManager
    {
        public:

        // Each resource has a handle locator, which maps names to handles which maps
        // to the POD (plain old data).
        HandleMap<Mesh, 8> MeshLocator;
        HandleMap<Shader, 8> ShaderLocator;
        HandleMap<Texture, 8> TextureLocator;

        static ResourceManager& GetInstance();
        ResourceManager() = default;
        ~ResourceManager();
        ResourceManager operator=(const ResourceManager&) = delete;
        ResourceManager(const ResourceManager&) = delete;
    
        // Registers a mesh with the GPU under the provided name.
        void SubmitMesh(const std::string& meshName, const void* pVertexData, 
                        usize vertexCount, usize vertexSize);

        // Registers a pre-compiled shader under the provided name.
        void SubmitShader(const std::string& shaderName,
                          const void* pVertexShaderData, usize vertexShaderSize, 
                          const void* pPixelShaderData, usize pixelShaderSize);
        
        // Submits an image to the GPU as a texture under the provided name.
        void SubmitTexture(const std::string& textureName, const Asset::Image& image);

        // Releases the resource. NOTE: ALL resources get released on shutdown.
        // This is for runtime releasing needs.
        void ReleaseMesh(const std::string& meshName);
        void ReleaseShader(const std::string& shaderName);
        void ReleaseTexture(const std::string& textureName);
    };
}