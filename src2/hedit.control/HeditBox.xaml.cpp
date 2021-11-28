#include "pch.h"
#include <debugapi.h>
#include "HeditBox.xaml.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;

namespace Hedit::Control {
    HeditBox::HeditBox() {
        InitializeComponent();
        CoreWindow^ window = Window::Current->CoreWindow;
        window->VisibilityChanged +=
            ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &HeditBox::OnVisibilityChanged);
        DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();
        currentDisplayInformation->DpiChanged +=
            ref new TypedEventHandler<DisplayInformation^, Object^>(this, &HeditBox::OnDpiChanged);

        currentDisplayInformation->OrientationChanged +=
            ref new TypedEventHandler<DisplayInformation^, Object^>(this, &HeditBox::OnOrientationChanged);

        DisplayInformation::DisplayContentsInvalidated +=
            ref new TypedEventHandler<DisplayInformation^, Object^>(this, &HeditBox::OnDisplayContentsInvalidated);

        swapchainPanel->CompositionScaleChanged +=
            ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &HeditBox::OnCompositionScaleChanged);

        swapchainPanel->SizeChanged +=
            ref new SizeChangedEventHandler(this, &HeditBox::OnSwapChainPanelSizeChanged);

        // Loaded and Unloaded
        this->Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &Hedit::Control::HeditBox::OnLoaded);
        this->Unloaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &Hedit::Control::HeditBox::OnUnloaded);

        // At this point we have access to the device. 
        // We can create the device-dependent resources.
        _dres = std::make_unique<DX::DeviceResources>();
        _dres->SetSwapChainPanel(swapchainPanel);

        _doc = ref new Hedit::Control::Document();
        _txtcore = ref new Hedit::Control::TextCorePart(this, _doc);

        _doc->TextUpdated += ref new Hedit::Control::TextUpdatedEventHandler(this, &Hedit::Control::HeditBox::OnTextUpdated);
    }

    void HeditBox::OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args) {
    }

    void HeditBox::OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args) {
        _dres->SetDpi(sender->LogicalDpi);
    }

    void HeditBox::OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args) {
    }

    void HeditBox::OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args) {
        _dres->ValidateDevice();
    }

    void HeditBox::OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args) {
        _dres->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
    }

    void HeditBox::OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e) {
        _dres->SetLogicalSize(e->NewSize);
        _vrect.UpdateView(e->NewSize.Width, e->NewSize.Height);
    }

    void HeditBox::OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
        _txtcore->Initialize();
        _vrect.Initialize(_doc, {}, _dres.get());
    }

    void HeditBox::OnUnloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
        _vrect.Uninitialize();
        _txtcore->Uninitialize();
    }

    void HeditBox::OnTextUpdated(Hedit::Control::Document^ sender) {
        dbgTextArea->Text = sender->FullText();
    }
}

