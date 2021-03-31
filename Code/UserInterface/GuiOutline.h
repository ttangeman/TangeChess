#pragma once

#include "UserInterface/GuiObject.h"

namespace Tange
{
    class GuiOutline : public GuiObject
    {
        // TODO: This should probably be a handle or some
        // kind of smart pointer.
        GuiObject* pParent;
        float m_outlineThickness = 0;
        Vec4 m_outlineColor = Vec4(1, 1, 1, 1);
        
    public:
        void SetOutline(float thickness)
        {
            m_outlineThickness = thickness;
        }

        void SetOutline(float thickness, Vec4 color)
        {
            m_outlineThickness = thickness;
            m_outlineColor = color;
        }
    };
}