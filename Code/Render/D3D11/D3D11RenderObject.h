#pragma once

#include "Utility/Handle.h"
#include "Render/RenderObject.h"
#include "Render/ResourceManager.h"

namespace Render
{
    class RenderObject
    {
        public:
            
        RenderObject() = default;

        void AttachShader(const std::string& shaderName);
        void AttachTexture(const std::string& textureName);
        void AttachMesh();

        private:

        Handle<Shader> m_hShader;
        Handle<Texture> m_hTexture;
        ID3D11Buffer* m_pVertexBuffer;
        ID3D11Buffer* m_pTransform;
        XMMATRIX m_projection;
    };
}