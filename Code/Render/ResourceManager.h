#pragma once

#include "Asset/Image.h"
#include "Utility/ResourceMap.h"
#include "Render/Renderer.h"

namespace Render
{
    class ResourceManager
    {
        public:

        // Each resource has a handle locator, which maps names to handles which maps
        // to the POD (plain old data).
        ResourceMap<Mesh> MeshLocator;
        ResourceMap<Shader> ShaderLocator;
        ResourceMap<Texture> TextureLocator;

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
    };
}