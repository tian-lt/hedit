#pragma once

#include "Document.h"
#include "DocumentVisualRect.h"
#include "DX/DeviceResources.h"
#include "HeditBox.g.h"

namespace Hedit::Control {
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HeditBox sealed {
	public:
		HeditBox();

	public:
		property Hedit::Control::Document^ Document {
			Hedit::Control::Document^ get() {
				return _doc;
			}
		}

	public:
		// Window event handlers.
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

		// Other event handlers.
		void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
		void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

	private:
		Hedit::Control::Document^ _doc;

	private:
		std::unique_ptr<DX::DeviceResources> _dres;
		DocumentVisualRect _vrect;
	};
}
