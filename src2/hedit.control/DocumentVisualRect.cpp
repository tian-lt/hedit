#include "pch.h"
#include "DocumentVisualRect.h"

namespace Hedit::Control {
    void DocumentVisualRect::Initialize(const TextStyleInfo& text_style_info, DX::DeviceResources* device_res) {
        _dres = device_res;
        _text_style_info = text_style_info;
    }
}

