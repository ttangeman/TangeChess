#pragma once

#include "Core/Common.h"
#include "Core/EventManager.h"
#include "Platform/InputEvents.h"
#include "Platform/PlatformManager.h"
#include "Entity/EntityManager.h"
#include "Entity/EntityEvents.h"
#include "Render/Drawable.h"
#include "Render/Transformable.h"
#include "Render/ResourceManager.h"
#include "UserInterface/GuiComponents.h"

namespace Tange
{
    class GuiObject
    {
        std::vector<Entity> m_entities;

    public:
        GuiObject() = default;
        ~GuiObject();

        void AddPanel(Vec2 position, Vec2 scale, bool selectable = true,
                      bool dragable = true, bool snapping = false);
    };
}