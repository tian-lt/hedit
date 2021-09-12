//
// HeditControl.xaml.cpp
// Implementation of the HeditControl class
//

#include "pch.h"
#include "HeditControl.xaml.h"

using namespace HeditControls;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

HeditControl::HeditControl()
	: m_windowVisible(true)
{
	InitializeComponent();
	CoreWindow^ window = Window::Current->CoreWindow;
	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &HeditControl::OnVisibilityChanged);
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();
	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &HeditControl::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &HeditControl::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &HeditControl::OnDisplayContentsInvalidated);

	swapchainPanel->CompositionScaleChanged += 
		ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &HeditControl::OnCompositionScaleChanged);

	swapchainPanel->SizeChanged +=
		ref new SizeChangedEventHandler(this, &HeditControl::OnSwapChainPanelSizeChanged);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->SetSwapChainPanel(swapchainPanel);

	// create renderer
	m_renderer = std::make_unique<GraphicsRenderer>(m_deviceResources.get());

	Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &HeditControls::HeditControl::OnLoaded);
	Unloaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &HeditControls::HeditControl::OnUnloaded);
}

void HeditControls::HeditControl::OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void HeditControls::HeditControl::OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args)
{
	m_deviceResources->SetDpi(sender->LogicalDpi);
}

void HeditControls::HeditControl::OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args)
{
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
}

void HeditControls::HeditControl::OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args)
{
	m_deviceResources->ValidateDevice();
}

void HeditControls::HeditControl::OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args)
{
	m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
}

void HeditControls::HeditControl::OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
}

void HeditControls::HeditControl::OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_renderer->StartRenderLoop();
}

void HeditControls::HeditControl::OnUnloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_renderer->StopRenderLoop();
}

