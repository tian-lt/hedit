#pragma once
#include "pch.h"

namespace Hedit::Control {
    ref class Document;
	[Windows::Foundation::Metadata::WebHostHidden]
    public delegate void TextUpdatedEventHandler(Hedit::Control::Document^ sender);

	[Windows::Foundation::Metadata::WebHostHidden]
    public ref class Document sealed {
    public:
        event TextUpdatedEventHandler^ TextUpdated;
        property Windows::UI::Text::Core::CoreTextRange Selection {
            Windows::UI::Text::Core::CoreTextRange get() {
                return _sel;
            }
        }

    public:
        void UpdateText(
            Windows::UI::Text::Core::CoreTextRange prev_sel,
            Platform::String^ text,
            Windows::UI::Text::Core::CoreTextRange new_sel);
        Platform::String^ FullText();

    private:
        std::vector<uint32_t> _content;
        Windows::UI::Text::Core::CoreTextRange _sel;
    };
}

