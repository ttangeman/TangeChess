#include "Render/D3D11/D3D11Renderer.h"

namespace Render
{
    void Quad::SetTexCoords(Vec2 min, Vec2 max)
    {
        Vertices[0].TexCoord = Vec2(min.U, min.V);
        Vertices[1].TexCoord = Vec2(min.U, max.V);
        Vertices[2].TexCoord = Vec2(max.U, max.V);

        Vertices[3].TexCoord = Vec2(max.U, max.V);
        Vertices[4].TexCoord = Vec2(max.U, min.V);
        Vertices[5].TexCoord = Vec2(min.U, min.V);
    }

    void InitializePipeline()
    {
        auto& platform = Platform::PlatformManager::Get();
        auto hWindow = platform.GetWindow();

        // Initialize D3D swap chain and devices.
        HRESULT result;
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
#if 0
        // Flip fullscreen
        swapChainDesc.BufferCount = BackBufferCount;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = hWindow;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        // NOTE: Multisampling cannot be utilized with flip fullscreen. 
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        // TODO: Query refresh rate properly.
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.Windowed = true;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#else
        // Exclusive fullscreen
        swapChainDesc.BufferCount = BackBufferCount;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.OutputWindow = hWindow;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.SampleDesc.Count = 4;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        // TODO: Query refresh rate properly.
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.Windowed = true;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif
        
        D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
        
        result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                               0, &featureLevel, 1, D3D11_SDK_VERSION, 
                                               &swapChainDesc, &g_pSwapChain, &g_pDevice, 
                                               nullptr, &g_pDeviceContext);
        CHECK_RESULT(result);
        
        // Clear the back buffer and adjust the view port.
        ClearRenderTarget();
        SetViewport(platform.GetRenderDimensions());
        
        // Initialize texture sampler.
        ID3D11SamplerState* pDefaultSampler;
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        
        result = g_pDevice->CreateSamplerState(&samplerDesc, &pDefaultSampler);
        CHECK_RESULT(result);
        
        g_pDeviceContext->PSSetSamplers(0, 1, &pDefaultSampler);
        
        ID3D11BlendState* pBlendState;
        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.RenderTarget[0].BlendEnable = true;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        
        g_pDevice->CreateBlendState(&blendDesc, &pBlendState);
        g_pDeviceContext->OMSetBlendState(pBlendState, nullptr, 0xffffffff);
    }
    
    void ShutdownPipeline()
    {
        // NOTE: To avoid thread contention, go windowed before releasing the swap chain.
        g_pSwapChain->SetFullscreenState(false, nullptr);
        SAFE_RELEASE(g_pRenderTarget);
        SAFE_RELEASE(g_pDeviceContext);
        SAFE_RELEASE(g_pDevice);
        SAFE_RELEASE(g_pSwapChain);
    }
    
    Transform CreateTransform()
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX) * 3;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        ID3D11Buffer* pTransformBuffer;
        HRESULT result = g_pDevice->CreateBuffer(&bufferDesc, nullptr, &pTransformBuffer);
        CHECK_RESULT(result);
        
        D3D11_MAPPED_SUBRESOURCE data;
        result = g_pDeviceContext->Map(pTransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
        CHECK_RESULT(result);
        
        TransformData* pTransformData = (TransformData*)data.pData;
        pTransformData->World = DirectX::XMMatrixIdentity();
        pTransformData->View = DirectX::XMMatrixIdentity();
        pTransformData->Projection = DirectX::XMMatrixIdentity();
        
        g_pDeviceContext->Unmap(pTransformBuffer, 0);

        Transform transform = {};
        transform.Projection = DirectX::XMMatrixIdentity();
        transform.pTransformBuffer = pTransformBuffer;
        
        return transform;
    }

    void PresentFrame()
    {
        HRESULT result = g_pSwapChain->Present(0, 0);

        if (result == DXGI_ERROR_DEVICE_REMOVED) 
        {
            result = g_pDevice->GetDeviceRemovedReason();
            CHECK_RESULT(result);
        }

        CHECK_RESULT(result);
        g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTarget, nullptr);
    }

    void FullClear(Vec4 clearColor)
    {
        g_pDeviceContext->ClearRenderTargetView(g_pRenderTarget, clearColor.Data);
    }

    void SetShader(const std::string& shaderName)
    {
        auto& resourceManager = ResourceManager::Get();
        auto hShader = resourceManager.ShaderLocator.GetResourceHandle(shaderName);
        const Shader& shader = resourceManager.ShaderLocator.LookupResource(hShader);
        
        g_pDeviceContext->VSSetShader(shader.pVertexShader, nullptr, 0);
        g_pDeviceContext->PSSetShader(shader.pPixelShader, nullptr, 0);
        g_pDeviceContext->IASetInputLayout(shader.pInputLayout);
    } 

    void SetViewport(Vec2 dimensions)
    {
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = dimensions.Width;
        viewport.Height = dimensions.Height;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        
        g_pDeviceContext->RSSetViewports(1, &viewport);
    }
    void ResizeWindow(float desiredWidth, float desiredHeight)
    {
        BOOL isFullscreen;
        g_pSwapChain->GetFullscreenState(&isFullscreen, nullptr);
        
        if (isFullscreen) 
        {
            g_pSwapChain->SetFullscreenState(false, nullptr);
            ResizeBackBuffers(desiredWidth, desiredHeight);
            ResizeTarget(desiredWidth, desiredHeight);
            g_pSwapChain->SetFullscreenState(true, nullptr);
        } 
        else 
        {
            ResizeBackBuffers(desiredWidth, desiredHeight);
            ResizeTarget(desiredWidth, desiredHeight);
        }
        
        SetViewport(Vec2(desiredWidth, desiredHeight));
    }

    void ToggleFullscreen() 
    {
        BOOL isFullscreen;
        g_pSwapChain->GetFullscreenState(&isFullscreen, nullptr);
        g_pSwapChain->SetFullscreenState(!isFullscreen, nullptr);
    }

    void ForceWindowed()
    {
        g_pSwapChain->SetFullscreenState(false, nullptr);
    }

    void ForceFullscreen()
    {
        g_pSwapChain->SetFullscreenState(true, nullptr);
    }
    
    bool IsFullscreen() 
    {
        BOOL isFullscreen;
        g_pSwapChain->GetFullscreenState(&isFullscreen, nullptr);
        return isFullscreen;
    }

    void ResizeBackBuffers(uint32 desiredWidth, uint32 desiredHeight)
    {
        g_pDeviceContext->OMSetRenderTargets(0, 0, 0);
        SAFE_RELEASE(g_pRenderTarget);
        g_pSwapChain->ResizeBuffers(0, desiredWidth, desiredHeight, DXGI_FORMAT_UNKNOWN, 0);

        ClearRenderTarget();
        SetViewport(Vec2((float)desiredWidth, (float)desiredHeight));
    }

    void ResizeTarget(uint32 desiredWidth, uint32 desiredHeight)
    {
        DXGI_MODE_DESC desiredDesc = {};
        desiredDesc.Width = desiredWidth;
        desiredDesc.Height = desiredHeight;
        g_pSwapChain->ResizeTarget(&desiredDesc);
    }

    void ClearRenderTarget()
    {
        HRESULT result;
        ID3D11Texture2D* pBackBuffer;
        
        result = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        CHECK_RESULT(result);
        
        result = g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTarget);
        SAFE_RELEASE(pBackBuffer);
        CHECK_RESULT(result);
        
        g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTarget, nullptr);//depth_stencil_view);
    }
}