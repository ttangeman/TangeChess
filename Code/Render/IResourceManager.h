#pragma once

#include "Core/Common.h"
#include "Platform/PlatformWrapper.h"

namespace Render
{
    class ShaderHandle;
    class TextureHandle;
    
    class IResourceManager
    {
        public:

        // Registers a pre-compiled shader with D3D11.
        virtual ShaderHandle SubmitShader(const std::string& vertexShaderByteCode, 
                                          const std::string& pixelShaderByteCode) = 0;
        
        // Submits an image to the GPU as a texture.
        virtual TextureHandle SubmitTexture() = 0; 
    };
}