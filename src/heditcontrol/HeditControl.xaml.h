//
// HeditControl.xaml.h
// Declaration of the HeditControl class
//

#pragma once

#include "HeditControl.g.h"
#include "DeviceResources.h"
#include "GraphicsRenderer.h"

namespace HeditControls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HeditControl sealed
	{
	public:
		HeditControl();

	private:
		// Window event handlers.
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

		// Other event handlers.
		void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
		void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

		void OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnUnloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		// Resources used to render the DirectX content in the XAML page background.
		std::unique_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<GraphicsRenderer> m_renderer;
		bool m_windowVisible;
	};
}
