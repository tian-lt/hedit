#pragma once
#include "DXCommon/DeviceResources.h"

namespace heditexp {
    class FontResources final{
    public:
        explicit FontResources(DX::DeviceResources* deviceResources);
    private:
        DX::DeviceResources* m_deviceResources;
        Microsoft::WRL::ComPtr<IDWriteFontSet> m_sysFontSet;
        Microsoft::WRL::ComPtr<IDWriteFontFace3> m_defaultFontFace;
    };

    class DWExp {
    };
}

