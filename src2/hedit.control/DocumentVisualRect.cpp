#include "pch.h"
#include "DocumentVisualRect.h"

namespace Hedit::Control {
    DocumentVisualRect::DocumentVisualRect() {
        _on_text_updated = ref new FuncDelegate<TextUpdatedEventHandler, void(Document^)>([this](auto sender) { OnTextUpdated(sender); });
    }

    void DocumentVisualRect::Initialize(Hedit::Control::Document^ doc, const TextStyleInfo& text_style_info, DX::DeviceResources* device_res) {
        _dres = device_res;
        _text_style_info = text_style_info;
        _x = 0.f; _y = 0.f; _w = 0.f; _h = 0.f;
        _sel = { 0, 0 };
        _doc = doc;
        _et_text_updated = _doc->TextUpdated += _on_text_updated->Handler;
    }
    
    void DocumentVisualRect::Uninitialize() {
        _doc->TextUpdated -= _et_text_updated;
        _doc = nullptr;
    }

    void DocumentVisualRect::UpdateView(float width, float height) {
        _w = width;
        _h = height;
    }

    void DocumentVisualRect::OnTextUpdated(Hedit::Control::Document^ sender) {
        int c = 0;
    }
}

