#pragma once

// NOTE: Implements all of the functions from this header.
#include "Render/Renderer.h"
#include "Render/D3D11/D3D11ResourceManager.h"

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

#define CheckResult(result) Assert(!(result));
#define SafeRelease(object) if (object) {object->Release();}

namespace Render
{
    static const int32 BackBufferCount = 2;

    // All of the D3D11 API-specific objects are kept global because
    // they are opaque classes anyways and are pretty much just used
    // to call API functions. However, if multi-threading ever becomes
    // a concern, then more caution should be warranted.
   
    static IDXGISwapChain* g_pSwapChain;
    static ID3D11Device* g_pDevice;
    static ID3D11DeviceContext* g_pDeviceContext;
    static ID3D11RenderTargetView* g_pRenderTarget;
}