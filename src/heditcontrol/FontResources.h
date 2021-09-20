#pragma once
#include "pch.h"
#include "DeviceResources.h"

namespace HeditControls
{
    class FontResources final
    {
    public:
        explicit FontResources(DX::DeviceResources* deviceResources);

    private:
        DX::DeviceResources* m_deviceResources;
        Microsoft::WRL::ComPtr<IDWriteFontSet> m_sysFontSet;
        Microsoft::WRL::ComPtr<IDWriteFontFace3> m_defaultFontFace;
    };
}


