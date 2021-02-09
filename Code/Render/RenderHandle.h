#pragma once

#include "Utility/Handle.h"
#include "Render/Renderer.h"
#include "Render/ResourceManager.h"

namespace Tange
{
    struct RenderHandle
    {
        Handle<Mesh> hMesh = {};
        Handle<Texture> hTexture = {};
        std::shared_ptr<GpuBuffer> pColorBuffer = nullptr;

        RenderHandle()
        {
            pColorBuffer = std::make_shared<GpuBuffer>(sizeof(Vec4));
            // Set the color to a sane default.
            SetColor(Vec4(0, 0, 0, 1));
        }

        void SetColor(Vec4 color)
        {
            auto* pBuffer = static_cast<Vec4*>(pColorBuffer->MapBuffer());
            pBuffer[0] = color;
            pColorBuffer->Unmap();
        }

        void AttachMesh(const std::string& meshName)
        {
            hMesh = ResourceManager::MeshLocator.GetResourceHandle(meshName);
        }

        void AttachTexture(const std::string& textureName)
        {
            hTexture = ResourceManager::TextureLocator.GetResourceHandle(textureName);
        }

        void AttachMaterial()
        {

        }

        const Mesh& GetMesh() const
        {
            return ResourceManager::MeshLocator.LookupResource(hMesh);
        }

        const Texture& GetTexture() const
        {
            return ResourceManager::TextureLocator.LookupResource(hTexture);
        }
    };
}