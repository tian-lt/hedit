#pragma once
#include "Document.h"

namespace Hedit::Control {
	[Windows::Foundation::Metadata::WebHostHidden]
    public ref class TextCorePart sealed {
	public:
		TextCorePart(Windows::UI::Xaml::Controls::Control^ owner, Hedit::Control::Document^ doc);

	public:
		void Initialize();
		void Uninitialize();
		void NotifyFocusLost();

	private:
		// event callbacks
		void OnPointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnTextRequested(Windows::UI::Text::Core::CoreTextEditContext^ sender, Windows::UI::Text::Core::CoreTextTextRequestedEventArgs^ e);
        void OnSelectionRequested(Windows::UI::Text::Core::CoreTextEditContext^ sender, Windows::UI::Text::Core::CoreTextSelectionRequestedEventArgs^ e);
		void OnFocusRemoved(Windows::UI::Text::Core::CoreTextEditContext^ sender, Platform::Object^ object);
		void OnTextUpdating(Windows::UI::Text::Core::CoreTextEditContext^ sender, Windows::UI::Text::Core::CoreTextTextUpdatingEventArgs^ e);
		void OnSelectionUpdating(Windows::UI::Text::Core::CoreTextEditContext^, Windows::UI::Text::Core::CoreTextSelectionUpdatingEventArgs^);
		void OnFormatUpdating(Windows::UI::Text::Core::CoreTextEditContext^, Windows::UI::Text::Core::CoreTextFormatUpdatingEventArgs^);
		void OnLayoutRequested(Windows::UI::Text::Core::CoreTextEditContext^, Windows::UI::Text::Core::CoreTextLayoutRequestedEventArgs^);
		void OnCompositionStarted(Windows::UI::Text::Core::CoreTextEditContext^, Windows::UI::Text::Core::CoreTextCompositionStartedEventArgs^);
		void OnCompositionCompleted(Windows::UI::Text::Core::CoreTextEditContext^, Windows::UI::Text::Core::CoreTextCompositionCompletedEventArgs^);

	private:
		void SetInternalFocus();
		void RemoveInternalFocus();

	private:
		Windows::UI::Xaml::Controls::Control^ _owner;
		Windows::UI::Text::Core::CoreTextEditContext^ _edit_ctx;
		Platform::Agile<Windows::UI::Core::CoreWindow> _core_wnd;
		Platform::Agile<Windows::UI::ViewManagement::InputPane> _input_pane;
		Windows::Foundation::EventRegistrationToken _et_pointer_pressed;
		Windows::Foundation::EventRegistrationToken _et_text_requested;
		Windows::Foundation::EventRegistrationToken _et_selection_requested;
		Windows::Foundation::EventRegistrationToken _et_focus_removed;
		Windows::Foundation::EventRegistrationToken _et_text_updating;
		Windows::Foundation::EventRegistrationToken _et_selection_updating;
		Windows::Foundation::EventRegistrationToken _et_format_updating;
		Windows::Foundation::EventRegistrationToken _et_layout_requested;
		Windows::Foundation::EventRegistrationToken _et_composition_started;
		Windows::Foundation::EventRegistrationToken _et_composition_completed;

	private:
		bool _infocus;
		Hedit::Control::Document^ _doc;
    };
}

