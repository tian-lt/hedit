#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "DocPage.h"
#include "Sprite.h"

namespace HeditControls
{
    class GraphicsMain : public DX::IDeviceNotify
    {
    public:
        explicit GraphicsMain(DX::DeviceResources* deviceResources);
        void StartRenderLoop();
        void StopRenderLoop();
        Concurrency::critical_section& GetCriticalSection() { return m_mutexRendering; }
        void ResetWindowSizeDependentResources(uint32_t width = 0, uint32_t height = 0);

    private:
        void OnDeviceLost() override;
        void OnDeviceRestored() override;

    private:
        void PreRender();
        void FinalRender();

    private:
        DX::DeviceResources* m_deviceResources;
        Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
        Concurrency::critical_section m_mutexRendering;
        std::unique_ptr<DocumentPage> m_page1;
        uint32_t m_viewWidth;
        uint32_t m_viewHeight;

        // scene
        Sprite m_quad1;
        Sprite m_quad2;
    };
}


