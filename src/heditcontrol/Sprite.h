#pragma once
#include "pch.h"
#include "DeviceResources.h"

namespace HeditControls
{
    class Sprite final
    {
    public:
        struct Vertex
        {
            DirectX::XMFLOAT4 pos;
            DirectX::XMFLOAT2 texcoord;
        };

    public:
        explicit Sprite(DX::DeviceResources* deviceResources);
        ~Sprite();
        void CreateDeviceIndependentResources();
        void CreateDeviceDependentResources();
        void DestroyDeviceIndpendentResources();
        void DestroyDeviceDependentResources();
        void Render();
        void SetContent(ID3D11ShaderResourceView* contentView) { m_contentResView = contentView; }

    private:
        ID3D11ShaderResourceView* m_contentResView;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertBuffer;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vsShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_psShader;
        Microsoft::WRL::ComPtr<ID3DBlob> m_vsShaderBlob;
        Microsoft::WRL::ComPtr<ID3DBlob> m_psShaderBlob;

        std::array<Vertex, 4> m_vertData;
        DX::DeviceResources* m_deviceResources;
    };
}

