#pragma once

#include "Asset/Image.h"
#include "Utility/HandleMap.h"
#include "Render/Renderer.h"

namespace Tange
{
    class ResourceManager
    {
        static ResourceManager s_instance;

    public:
        // Each resource has a handle locator, which maps names to handles which maps
        // to the POD (plain old data).
        static HandleMap<Mesh, 32> MeshLocator;
        static HandleMap<Shader, 8> ShaderLocator;
        static HandleMap<Texture, 8> TextureLocator;

        ~ResourceManager();
        ResourceManager operator=(const ResourceManager&) = delete;
        ResourceManager(const ResourceManager&) = delete;

        // Initializes some default meshes.
        static void Initialize();

        // Registers a mesh with the GPU under the provided name.
        static void SubmitMesh(const std::string& meshName, const void* pVertexData, 
                               usize vertexCount, usize vertexSize);

        // Registers a pre-compiled shader under the provided name.
        static void SubmitShader(const std::string& shaderName,
                                 const void* pVertexShaderData, usize vertexShaderSize, 
                                 const void* pPixelShaderData, usize pixelShaderSize);
        
        // Submits an image to the GPU as a texture under the provided name.
        static void SubmitTexture(const std::string& textureName, const Image& image);

        // Releases the resource. NOTE: ALL resources get released on shutdown.
        // This is for runtime releasing needs.
        static void ReleaseMesh(const std::string& meshName);
        static void ReleaseShader(const std::string& shaderName);
        static void ReleaseTexture(const std::string& textureName);

    private:
        ResourceManager() = default;
    };
}