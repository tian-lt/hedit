//
// HeditControl.xaml.cpp
// Implementation of the HeditControl class
//

#include "pch.h"
#include "HeditControl.xaml.h"

using namespace HeditControls;

using namespace Concurrency;
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
	m_gmain = std::make_unique<GraphicsMain>(m_deviceResources.get());

	Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &HeditControl::OnLoaded);
	Unloaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &HeditControl::OnUnloaded);

	m_doc = ref new StaticTextDocument(ref new TextChangedDelegate(this, &HeditControl::DocTextChanged));
}

void HeditControls::HeditControl::OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void HeditControls::HeditControl::OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args)
{
	critical_section::scoped_lock lock(m_gmain->GetCriticalSection());
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_gmain->ResetWindowSizeDependentResources();
}

void HeditControls::HeditControl::OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args)
{
}

void HeditControls::HeditControl::OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args)
{
	critical_section::scoped_lock lock(m_gmain->GetCriticalSection());
	m_deviceResources->ValidateDevice();
}

void HeditControls::HeditControl::OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_gmain->GetCriticalSection());
	m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	m_gmain->ResetWindowSizeDependentResources();
}

void HeditControls::HeditControl::OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	critical_section::scoped_lock lock(m_gmain->GetCriticalSection());
	m_deviceResources->SetLogicalSize(e->NewSize);
	float w = e->NewSize.Width;
	float h = e->NewSize.Height;
	m_gmain->ResetWindowSizeDependentResources(std::lround(e->NewSize.Width), std::lround(e->NewSize.Height));
}

void HeditControls::HeditControl::OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_gmain->StartRenderLoop();
}

void HeditControls::HeditControl::OnUnloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_gmain->StopRenderLoop();
}

void HeditControls::HeditControl::DocTextChanged()
{

}

