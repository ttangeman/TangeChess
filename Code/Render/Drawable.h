#pragma once

#include "Utility/Handle.h"
#include "World/IComponent.h"
#include "Render/Renderer.h"
#include "Render/ResourceManager.h"
#include "Render/Transformable.h"
#include "World/EntityManager.h"

namespace Render
{
    // TODO: View matrix modification (camera?).
    class Drawable : public World::IComponent
    {
        public:
            
        Drawable();

        // Attach resources to the object.
        void AttachMesh(const std::string& meshName);
        void AttachTexture(const std::string& textureName);
        void AttachMaterial();

        // Draw the object.
        void Draw() const;

        private:

        Handle<Mesh> m_hMesh;
        Handle<Texture> m_hTexture;
    };
}