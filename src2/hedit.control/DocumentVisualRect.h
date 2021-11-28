#pragma once

#include "DX/DeviceResources.h"
#include "DocumentVisualTile.h"
#include "Document.h"
#include "Utils.h"

namespace Hedit::Control {
    struct TextStyleInfo {
        std::wstring font_family;
        std::wstring locale;
        float font_size;
    };

    struct TextRange {
        size_t beg;
        size_t end;
    };

    class DocumentVisualRect {
    public:
        DocumentVisualRect();
        DocumentVisualRect(const DocumentVisualRect&) = delete;
        DocumentVisualRect(DocumentVisualRect&&) = default;

    public:
        void Initialize(Hedit::Control::Document^ doc, const TextStyleInfo& text_style_info, DX::DeviceResources* device_res);
        void Uninitialize();
        void UpdateView(float width, float height);
        void UpdateText();
        void UpdateSelection();
        
    private:
        void OnTextUpdated(Hedit::Control::Document^ sender);

    private:
        enum class tile_index {
            primary = 0,
            primary_ext = 1,
            secondary = 2,
            secondary_ext = 3,

            // last
            tile_count
        };
        static inline constexpr size_t tile_count = static_cast<size_t>(tile_index::tile_count);

    private:
        FuncDelegate<Hedit::Control::TextUpdatedEventHandler, void(Hedit::Control::Document^)>^ _on_text_updated;
        Windows::Foundation::EventRegistrationToken _et_text_updated;

    private:
        DX::DeviceResources* _dres;
        TextStyleInfo _text_style_info;
        Hedit::Control::Document^ _doc;
        std::unique_ptr<DocumentVisualTile> _tiles[tile_count];
        TextRange _sel;
        float _x, _y, _w, _h;
    };
} // namespace Hedit::Control

