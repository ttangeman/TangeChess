#pragma once

#include "Utility/Handle.h"
#include "Render/Renderer.h"
#include "Render/ResourceManager.h"

namespace Render
{
    class RenderObject
    {
        public:
            
        RenderObject() = default;

        void AttachMesh(const std::string& meshName);
        void AttachTexture(const std::string& textureName);
        void AttachMaterial();

        void Draw() const;

        private:

        Handle<Mesh> m_hMesh;
        Handle<Texture> m_hTexture;
        // TODO: Make transform + camera class.
        //ID3D11Buffer* m_pTransform;
        //XMMATRIX m_projection;
    };
}