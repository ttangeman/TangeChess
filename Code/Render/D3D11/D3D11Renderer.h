#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

#define CHECK_RESULT(result) ASSERT(!(result));
#define SAFE_RELEASE(object) if (object) {object->Release();}

// All of the D3D11 API-specific objects are kept global because
// they are opaque classes anyways and are pretty much just used
// to call API functions. However, if multi-threading ever becomes
// a concern, then more caution should be warranted.
static IDXGISwapChain* g_pSwapChain;
static ID3D11Device* g_pDevice;
static ID3D11DeviceContext* g_pDeviceContext;
static ID3D11RenderTargetView* g_pRenderTarget;

#include "Render/D3D11/D3D11GpuBuffer.h"

namespace Tange
{
    using namespace DirectX;

    static const int32 g_BackBufferCount = 2;

#pragma pack(push, 1)
    struct Vertex
    {
        Vec3 Position;
        Vec4 Color;
        Vec2 TexCoord;
    };
    
    struct Quad
    {
        static const int32 VerticeCount = 6;

        Vertex Vertices[VerticeCount];
        
        static Quad Default();
        static Quad CreatePreTransformed(Vec2 minPosition, Vec2 maxPosition, Vec4 color, 
                                         Vec2 minUV = Vec2(), Vec2 maxUV = Vec2());

        void SetTexCoords(Vec2 minUV, Vec2 maxUV);
    };

    struct Mesh
    {
        uint32 Id;
        // See DeviceContext::IASetVertexBuffers for more info on these integers.
        uint32 VertexBufferSlot;
        uint32 VertexBufferCount;
        uint32 VertexBufferStride; // This can be an array.
        uint32 VertexBufferOffset; // This can be an array.
        // See DeviceContext::Draw for more info on these integers.
        uint32 VertexCount;
        uint32 StartVertexLocation;
        // GPU relevant data.
        ID3D11Buffer* pVertexBuffer;
        // TODO: Switch to indexed vertex buffers.
        // uint32 IndexCount;
        ID3D11Buffer* pIndexBuffer;
    };
    
    struct Shader
    {
        uint32 Id;
        // GPU relevant data.
        ID3D11VertexShader* pVertexShader;
        ID3D11PixelShader* pPixelShader;
        ID3D11InputLayout* pInputLayout;
    };
    
    struct Texture
    {
        uint32 Id;
        // GPU relevant data.
        ID3D11ShaderResourceView* pTextureView;
    };
    
    struct TransformData
    {
        XMMATRIX World;
        XMMATRIX View;
        XMMATRIX Projection;
    };

    struct Transform
    {
        // NOTE: Only projection needs to be stored.
        XMMATRIX Projection;
        // GPU relevant data.
        std::shared_ptr<GpuBuffer> pTransformBuffer;

        Transform();

        void WindowOrthographic();
        void Orthographic(Vec2 min, Vec2 max, float nearZ, float farZ);
        void Update(Vec3 position, Vec3 scale, Vec3 rotation);
    };
#pragma pack(pop)

    namespace Renderer
    {
        // Initailize/shutdown the render pipeline.
        void InitializePipeline();
        void ShutdownPipeline();

        // Swap the back buffers and present the frame.
        void PresentFrame();

        // Clear the entire screen with the color.
        void FullClear(Vec4 clearColor);

        // Set the active shader.
        void SetShader(const std::string& shaderName);

        // Set the viewport.
        void SetViewport(Vec2 dimensions);

        // Returns the current draw region.
        Vec2 GetDrawRegion();

        // Resizes the window
        void ResizeWindow(float desiredWidth, float desiredHeight);

        // Change from/to fullscreen.
        void ToggleFullscreen();
        void ForceWindowed();
        void ForceFullscreen();
        bool IsFullscreen();

        // Resize the back buffer/render target.
        void ResizeBackBuffers(uint32 desiredWidth, uint32 desiredHeight);
        void ResizeTarget(uint32 desiredWidth, uint32 desiredHeight);
        void ClearRenderTarget();
    }
}
