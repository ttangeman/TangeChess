#pragma once

#include "Core/Common.h"
#include "Entity/Component.h"

namespace Tange
{
    class GuiComponent : public Component<GuiComponent>
    {
    public:
        Rect BoundingBox;
        Vec2i PreviousMousePosition;
        bool Selected;
        bool ShouldSnap;
        bool Dragable;

        bool CollidesWith(Vec2i point)
        {
            if ((point.X < BoundingBox.MaxCorner.X && 
                point.Y < BoundingBox.MaxCorner.Y) &&
                (point.X > BoundingBox.MinCorner.X &&
                point.Y > BoundingBox.MinCorner.Y))
            {
                return true;
            } 
            return false;
        }

        void ComputeBoundingBox(Vec2 position, Vec2 scale)
        {
            // The scale _is_ the diameter.
            Vec2 radius = Vec2(scale.X / 2.0, scale.Y / 2.0);
            BoundingBox = Rect(position - radius, position + radius);
        }
    };
}