#pragma once

#include "Render/D3D11/D3D11Renderer.h"

namespace Tange
{
    enum BufferBinding
    {
        VertexShader,
        PixelShader,
        GeometryShader,
    };

    // Wrapper for D3D11 buffers.
    class GpuBuffer
    {
        ID3D11Buffer* m_pBuffer;

    public:
        GpuBuffer() = default;

        GpuBuffer(usize bufferSize)
        {
            CreateBuffer(bufferSize);
        }

        ~GpuBuffer()
        {
            ReleaseBuffer();
        }

        GpuBuffer(const GpuBuffer& copy)
        {
            std::memcpy(this, &copy, sizeof(*this));
        }
        
        GpuBuffer(GpuBuffer&& other)
        {
            this->m_pBuffer = other.m_pBuffer;
            other.m_pBuffer = nullptr;
        }

        void* MapBuffer()
        {
            D3D11_MAPPED_SUBRESOURCE data;
            HRESULT result = g_pDeviceContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
            CHECK_RESULT(result);

            return data.pData;
        }

        void Unmap()
        {
            g_pDeviceContext->Unmap(m_pBuffer, 0);
        }

        void SetBuffer(BufferBinding binding, uint32 slot) const
        {
            switch (binding)
            {
                case BufferBinding::VertexShader:
                    g_pDeviceContext->VSSetConstantBuffers(slot, 1, &m_pBuffer);
                break;

                case BufferBinding::PixelShader:
                    g_pDeviceContext->PSSetConstantBuffers(slot, 1, &m_pBuffer);
                break;

                case BufferBinding::GeometryShader:
                    g_pDeviceContext->GSSetConstantBuffers(slot, 1, &m_pBuffer);
                break;
            }
        }

    private:
        void CreateBuffer(usize bufferSize)
        {
            D3D11_BUFFER_DESC bufferDesc = {};
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            bufferDesc.ByteWidth = bufferSize;
            // TODO: Constant buffers only currently.
            // Make sure to change SetBuffer to!
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            HRESULT result = g_pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pBuffer);
            CHECK_RESULT(result);
        }

        void ReleaseBuffer()
        {
            SAFE_RELEASE(m_pBuffer);
        }
    };
}