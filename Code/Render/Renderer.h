#pragma once

#include "Core/Common.h"
#include "Platform/PlatformManager.h"
#include "Render/IResourceManager.h"

// NOTE: All of these functions should be implemented for
// each graphics API.
namespace Render
{
    // Initializes the renderer.
    void InitializePipeline(std::unique_ptr<IResourceManager>& pResourceManager);

    // Shutsdown the renderer.
    void Shutdown();
    
    // Displays the frame and swaps the back buffer.
    void PresentFrame();

    // Clears the screen with a color.
    void FullClear(Vec4 clearColor);
    
    // Resizes the application.
    void ResizeWindow(float desiredWidth, float desiredHeight); 

    // Changes/toggles whether the application is fullscreen or not.
    void ToggleFullscreen();
    void ForceFullscreen();
    void ForceWindowed();

    bool IsFullscreen();
    
    // Sets the viewport.
    void SetViewport(Vec2 dimensions);
    
    // Resizes the swap chain's back buffer size.
    void ResizeBackBuffers(uint32 desiredWidth, uint32 desiredHeight);

    // Resizes the output target.
    void ResizeTarget(uint32 desiredWidth, uint32 desiredHeight);

    // Clears the active render target.
    void ClearRenderTarget();
}