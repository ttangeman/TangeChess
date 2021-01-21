#pragma once

// NOTE: Implements all of the functions from this header.
#include "Render/Renderer.h"

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

    struct Vertex
    {
        Vec3 Position;
        Vec4 Color;
        Vec2 TexCoord;
    };
    
    struct Quad
    {
        Vertex Vertices[6];
    };
    
    struct Mesh
    {
        uint64 Id;
        ID3D11Buffer* pVertexBuffer;
        ID3D11Buffer* pIndexBuffer;
        
        Mesh(uint64 id, ID3D11Buffer* pVertexBuffer, ID3D11Buffer *pIndexbuffer);
        ~Mesh();
    };
    
    struct Shader
    {
        uint64 Id;
        ID3D11VertexShader* pVertexShader;
        ID3D11PixelShader* pPixelShader;
        ID3D11InputLayout* pInputLayout;

        ~Shader(); 
    };
    
    struct Texture
    {
        uint64 Id;
        ID3D11ShaderResourceView* pTextureView;
        
        Texture(uint64 id, ID3D11ShaderResourceView* pTextureView);
        ~Texture();
    };
    
}