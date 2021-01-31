#include "UserInterface/Widgets.h"

namespace Tange
{
    // TODO: For clickable gui elements, focus recognition for elements is
    // needed. Lets say there was a button overlapped on a panel, the
    // button and panel would both fire off their handler if the button
    // was clicked.

    // TODO: Need to set up an event for WM_RESIZE.

    // TODO: Normalize mouse coords?

    // TODO: FontManager or something that lets me query the
    // font atlas in here.

    IWidget::IWidget(IWidget&& other)
        : m_entity(other.m_entity)
    {
        // NOTE: Invalidates entity.
        other.m_entity = {};
    }

    IWidget::~IWidget()
    {
        if (m_entity.IsValid())
        {
            Hide();
        }
    }
    
    void IWidget::Hide()
    {
        auto& eventManager = EventManager::Get();
        eventManager.DetachAllHandlers(m_entity.Id);
    }

    Panel::Panel(Vec2 position, Vec2 scale, Vec4 color, bool interactable)
    {
        auto& entityManager = EntityManager::Get();
        auto& resourceManager = ResourceManager::Get();

        m_entity = entityManager.RegisterEntity();

        auto& drawable = entityManager.AttachComponent<Drawable>(m_entity);
        drawable.AttachMesh("DefaultQuad");
        drawable.SetColor(color);

        auto& transform = entityManager.AttachComponent<Transformable>(m_entity);
        transform.SetOrthographic(Vec2(), GetDrawRegion(), 0.1, 100.0);
        transform.Position = position;
        transform.Scale = Vec3(scale.X, scale.Y, 0);

        if (interactable)
        {
            auto& interactableComponent = entityManager.AttachComponent<MouseInteractable>(m_entity);
            interactableComponent.ComputeBoundingBox(position, scale);
        }
    }

    void Panel::Show()
    {
        auto& eventManager = EventManager::Get();
        auto& entityManager = EntityManager::Get();

        eventManager.BindHandler<UpdateEvent>(m_entity.Id,
        [this](const IEvent& event)
        {
            auto& entityManager = EntityManager::Get();
            
            auto& transform = entityManager.GetComponent<Transformable>(m_entity);
            transform.OnUpdate();
        });

        eventManager.BindHandler<RenderEvent>(m_entity.Id,
        [this](const IEvent& event)
        {
            // TODO: This is very inefficient. Need a way to 
            // build a render scene and set the shader for batches
            // of objects.
            SetShader("PixelFill");

            auto& entityManager = EntityManager::Get();

            auto& transform = entityManager.GetComponent<Transformable>(m_entity);
            transform.OnRender();

            auto& drawable = entityManager.GetComponent<Drawable>(m_entity);
            drawable.OnRender();
        });

        if (entityManager.HasComponent<MouseInteractable>(m_entity))
        {
            auto& interactable = entityManager.GetComponent<MouseInteractable>(m_entity);
            interactable.BindHandlers(true);
        }
    }
}