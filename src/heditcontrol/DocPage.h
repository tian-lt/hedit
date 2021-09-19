#pragma once
#include "pch.h"

namespace HeditControls
{
    class DocumentPage final
    {
    public:
        explicit DocumentPage(DX::DeviceResources* deviceResources);
        void CreateDeviceDependentResources();
        void ResetDeviceDependentResources();
        void DestroyDeviceDependentResources();
        void CreateWindowSizeDependentResources(uint32_t width = 0, uint32_t height = 0);
        void ResetWindowSizeDependentResources(uint32_t width = 0, uint32_t height = 0);
        void DestroyWindowSizeDependentResources();
        void Update();
        void Render();

        ID3D11ShaderResourceView* GetResourceView() const { return m_texOffScreenView.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_rtOffScreen;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texOffScreen;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texOffScreenView;
		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_brush;
		Microsoft::WRL::ComPtr<IDWriteTextLayout3> m_txtLayout;
        Microsoft::WRL::ComPtr<IDWriteTextFormat2> m_txtFormat;
        DX::DeviceResources* m_deviceResources;
        D2D1_SIZE_U m_imgSize;
    };
}

