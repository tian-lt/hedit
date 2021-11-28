#include "pch.h"
#include "TextCorePart.h"

using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Text::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;

namespace Hedit::Control {
    TextCorePart::TextCorePart(
        Windows::UI::Xaml::Controls::Control^ owner,
        Hedit::Control::Document^ doc) {
        _infocus = false;
        _owner = owner;
        _doc = doc;
        _core_wnd = CoreWindow::GetForCurrentThread();
        _input_pane = InputPane::GetForCurrentView();
        _edit_ctx = CoreTextServicesManager::GetForCurrentView()->CreateEditContext();
        _edit_ctx->InputPaneDisplayPolicy = CoreTextInputPaneDisplayPolicy::Manual;
        _edit_ctx->InputScope = CoreTextInputScope::Text;
    }

    void TextCorePart::Initialize() {
        _et_pointer_pressed = _core_wnd->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &TextCorePart::OnPointerPressed);
        _et_text_requested = _edit_ctx->TextRequested += ref new TypedEventHandler<CoreTextEditContext^, CoreTextTextRequestedEventArgs^>(this, &TextCorePart::OnTextRequested);
        _et_selection_requested = _edit_ctx->SelectionRequested += ref new TypedEventHandler<CoreTextEditContext^, CoreTextSelectionRequestedEventArgs^>(this, &TextCorePart::OnSelectionRequested);
        _et_focus_removed = _edit_ctx->FocusRemoved += ref new TypedEventHandler<CoreTextEditContext^, Platform::Object^>(this, &TextCorePart::OnFocusRemoved);
        _et_text_updating = _edit_ctx->TextUpdating += ref new TypedEventHandler<CoreTextEditContext^, CoreTextTextUpdatingEventArgs^>(this, &TextCorePart::OnTextUpdating);
        _et_selection_updating = _edit_ctx->SelectionUpdating += ref new TypedEventHandler<CoreTextEditContext^, CoreTextSelectionUpdatingEventArgs^>(this, &TextCorePart::OnSelectionUpdating);
        _et_format_updating = _edit_ctx->FormatUpdating += ref new TypedEventHandler<CoreTextEditContext^, CoreTextFormatUpdatingEventArgs^>(this, &TextCorePart::OnFormatUpdating);
        _et_layout_requested = _edit_ctx->LayoutRequested += ref new TypedEventHandler<CoreTextEditContext^, CoreTextLayoutRequestedEventArgs^>(this, &TextCorePart::OnLayoutRequested);
        _et_composition_started = _edit_ctx->CompositionStarted += ref new TypedEventHandler<CoreTextEditContext^, CoreTextCompositionStartedEventArgs^>(this, &TextCorePart::OnCompositionStarted);
        _et_composition_completed = _edit_ctx->CompositionCompleted += ref new TypedEventHandler<CoreTextEditContext^, CoreTextCompositionCompletedEventArgs^>(this, &TextCorePart::OnCompositionCompleted);
    }

    void TextCorePart::Uninitialize() {
        _core_wnd->PointerPressed -= _et_pointer_pressed;
        _edit_ctx->TextRequested -= _et_text_requested;
        _edit_ctx->SelectionRequested -= _et_selection_requested;
        _edit_ctx->FocusRemoved -= _et_focus_removed;
        _edit_ctx->TextUpdating -= _et_text_updating;
        _edit_ctx->SelectionUpdating -= _et_selection_updating;
        _edit_ctx->FormatUpdating -= _et_format_updating;
        _edit_ctx->LayoutRequested -= _et_layout_requested;
        _edit_ctx->CompositionStarted -= _et_composition_started;
        _edit_ctx->CompositionCompleted -= _et_composition_completed;
    }

    void TextCorePart::NotifyFocusLost() {
        RemoveInternalFocus();
    }

    void TextCorePart::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ e) {
        SetInternalFocus();
        _owner->Focus(FocusState::Programmatic);
    }

    void TextCorePart::OnTextRequested(CoreTextEditContext^ sender, CoreTextTextRequestedEventArgs^ e) {
    }

    void TextCorePart::OnSelectionRequested(CoreTextEditContext^ sender, CoreTextSelectionRequestedEventArgs^ e) {
    }

    void TextCorePart::OnFocusRemoved(CoreTextEditContext^ sender, Platform::Object^ object) {
    }

    void TextCorePart::OnTextUpdating(CoreTextEditContext^ sender, CoreTextTextUpdatingEventArgs^ args) {
        CoreTextRange sel = args->Range;
        CoreTextRange newsel = args->NewSelection;
        Platform::String^ text = args->Text;
        _doc->UpdateText(sel, text, newsel);
    }

    void TextCorePart::OnSelectionUpdating(CoreTextEditContext^ sender, CoreTextSelectionUpdatingEventArgs^ e) {
    }

    void TextCorePart::OnFormatUpdating(CoreTextEditContext^ sender, CoreTextFormatUpdatingEventArgs^ e) {
    }

    void TextCorePart::OnLayoutRequested(CoreTextEditContext^ sender, CoreTextLayoutRequestedEventArgs^ e) {
    }

    void TextCorePart::OnCompositionStarted(CoreTextEditContext^ sender, CoreTextCompositionStartedEventArgs^ e) {
    }

    void TextCorePart::OnCompositionCompleted(CoreTextEditContext^ sender, CoreTextCompositionCompletedEventArgs^ e) {
    }

    void TextCorePart::SetInternalFocus() {
        if (!_infocus) {
            _infocus = true;
            _edit_ctx->NotifyFocusEnter();
        }
        _input_pane->TryShow();
    }

    void TextCorePart::RemoveInternalFocus() {
        if (_infocus) {
            _infocus = false;
            _edit_ctx->NotifyFocusLeave();
            _input_pane->TryHide();
        }
    }
}
