#pragma once

#include "Core/Common.h"
#include "Entity/Entity.h"
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
    class IWidget
    {
    protected:
        Entity m_entity;

    public:
        IWidget() = default;
        IWidget(IWidget&& other);
        virtual ~IWidget();

        virtual void Show() = 0;
        virtual void Hide();
    };

    // A panel is just a rectangle that can be selected/dragged.
    class Panel : public IWidget
    {
    public:
        Panel() = default;
        Panel(Vec2 position, Vec2 scale, Vec4 color, bool interactable = true);
        
        void Show() override;
    };
}