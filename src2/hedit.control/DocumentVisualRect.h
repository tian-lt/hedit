#pragma once

#include "DX/DeviceResources.h"
#include "DocumentVisualTile.h"

namespace Hedit::Control {
    struct TextStyleInfo {
        std::wstring font_family;
        std::wstring locale;
        float font_size;
    };

    class DocumentVisualRect {
    public:
        void Initialize(const TextStyleInfo& text_style_info, DX::DeviceResources* device_res);

    private:
        enum class tile_index {
            left_top = 0,
            right_top = 1,
            left_bottom = 2,
            right_bottom = 3,

            // last
            tile_count
        };
        static inline constexpr size_t tile_count = static_cast<size_t>(tile_index::tile_count);

    private:
        DX::DeviceResources* _dres;
        TextStyleInfo _text_style_info;
        std::unique_ptr<DocumentVisualTile> _tiles[tile_count];
    };
} // namespace Hedit::Control

