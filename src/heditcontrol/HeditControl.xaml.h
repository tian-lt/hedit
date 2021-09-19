//
// HeditControl.xaml.h
// Declaration of the HeditControl class
//

#pragma once

#include "HeditControl.g.h"
#include "TextDocument.h"
#include "DeviceResources.h"
#include "GraphicsMain.h"

namespace HeditControls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HeditControl sealed
	{
	public:
		HeditControl();
		property ITextDocument^ Document
		{
			ITextDocument^ get() { return m_doc; }
			void set(ITextDocument^ value) { m_doc = value; }
		}

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

		void DocTextChanged();

	private:
		ITextDocument^ m_doc;

	private:
		// Resources used to render the DirectX content in the XAML page background.
		std::unique_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<GraphicsMain> m_gmain;
		bool m_windowVisible;
	};
}
