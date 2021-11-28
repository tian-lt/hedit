#include "pch.h"
#include "Document.h"

namespace Hedit::Control {
    void Document::UpdateText(
            Windows::UI::Text::Core::CoreTextRange prev_sel,
            Platform::String^ text,
            Windows::UI::Text::Core::CoreTextRange new_sel) {
        // TODO: optimize below with better memory arrangements
        _content.erase(
            _content.cbegin() + prev_sel.StartCaretPosition,
            _content.cbegin() + prev_sel.EndCaretPosition);
        _content.insert(_content.cbegin() + prev_sel.StartCaretPosition, text->Length(), 0);
        auto iter = _content.begin() + prev_sel.StartCaretPosition;
        auto data = text->Data();
        for (size_t i = 0; i < text->Length(); ++i, ++iter) {
            *iter = static_cast<uint32_t>(data[i]);
        }
        _sel = new_sel;
        TextUpdated(this);
    }

    Platform::String^ Document::FullText() {
        // note: this method is really costly.
        std::wstring wstr;
        wstr.resize(_content.size());
        for (size_t i = 0; i < _content.size(); ++i) {
            wstr[i] = static_cast<wchar_t>(_content[i]);
        }
        return ref new Platform::String(wstr.c_str());
    }
}
