#pragma once

namespace HeditControls
{
    class GraphicsRenderer : public DX::IDeviceNotify
    {
    public:
        explicit GraphicsRenderer(DX::DeviceResources* deviceResources);
        void StartRenderLoop();
        void StopRenderLoop();
        Concurrency::critical_section& GetCriticalSection() { return m_mutexRendering; }

    private:
		// IDeviceNotify
        void OnDeviceLost() override {}
        void OnDeviceRestored() override {}

    private:
        void Render();

    private:
        DX::DeviceResources* m_deviceResources;
        Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
        Concurrency::critical_section m_mutexRendering;
    };
}


