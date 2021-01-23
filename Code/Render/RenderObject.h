#pragma once

#include "Utility/Handle.h"
#include "Render/Renderer.h"
#include "Render/ResourceManager.h"

namespace Render
{
    // TODO: View matrix modification (camera?).
    class RenderObject
    {
        public:
            
        RenderObject();

        // Attach resources to the object.
        void AttachMesh(const std::string& meshName);
        void AttachTexture(const std::string& textureName);
        void AttachMaterial();

        // Changes to orthographic mode.
        void SetOrthographic(Vec2 minView, Vec2 maxView, float nearZ, float farZ);

        // Transforms that can be done to the object.
        void Translate(Vec3 position);
        void Scale(Vec3 scaling);
        void Rotate(Vec3 rotation);

        // Updates the GPU buffer with the transformed matrices.
        void Update();

        // Draw the object.
        void Draw() const;

        private:

        Handle<Mesh> m_hMesh;
        Handle<Texture> m_hTexture;
        std::unique_ptr<Transform> m_pTransform;
    };
}