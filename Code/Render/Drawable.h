#pragma once

#include "Utility/Handle.h"
#include "Entity/Component.h"
#include "Entity/EntityManager.h"
#include "Render/Renderer.h"
#include "Render/ResourceManager.h"
#include "Render/Transformable.h"

namespace Tange
{
    // TODO: View matrix modification (camera?).
    class Drawable : public Component<Drawable>
    {
        Handle<Mesh> m_hMesh;
        Handle<Texture> m_hTexture;
        
    public:
        // Attach resources to the object.
        void AttachMesh(const std::string& meshName);
        void AttachTexture(const std::string& textureName);
        void AttachMaterial();

        // Draw the object.
        void OnRender() const;
    };
}