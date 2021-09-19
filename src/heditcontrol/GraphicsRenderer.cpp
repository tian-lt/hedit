#include "pch.h"
#include "DeviceResources.h"
#include "DirectXHelper.h"
#include "DocPage.h"
#include "GraphicsRenderer.h"

namespace DX = HeditControls::DX;

using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace HeditControls;

GraphicsRenderer::GraphicsRenderer(DX::DeviceResources* deviceResources)
	: m_deviceResources(deviceResources)
	, m_viewWidth(0)
	, m_viewHeight(0)
	, m_page1(std::make_unique<DocumentPage>(deviceResources))
	, m_quad1(deviceResources)
	, m_quad2(deviceResources)
{
	auto d3ddev = m_deviceResources->GetD3DDevice();
}

void GraphicsRenderer::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
	{
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_mutexRendering);
			m_page1->Update();
			PreRender();
			m_page1->Render();
			FinalRender();
			wait(16); // TODO: keep staying at 60 FPS
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void GraphicsRenderer::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

void GraphicsRenderer::ResetWindowSizeDependentResources(uint32_t width, uint32_t height)
{
	if (width > 0 && height > 0)
	{
		m_viewWidth = width;
		m_viewHeight = height;
	}
	m_page1->ResetWindowSizeDependentResources(width, height);
}

void GraphicsRenderer::OnDeviceLost()
{
	m_page1->DestroyDeviceDependentResources();
	m_quad1.DestroyDeviceDependentResources();
	m_quad2.DestroyDeviceDependentResources();
}

void GraphicsRenderer::OnDeviceRestored()
{
	m_quad1.CreateDeviceDependentResources();
	m_quad2.CreateDeviceDependentResources();
	m_page1->CreateDeviceDependentResources();
}

void GraphicsRenderer::PreRender()
{
    auto ctx = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	ctx->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	ctx->OMSetRenderTargets(1, targets, nullptr);

	// Clear the back buffer view.
	ctx->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::Gray);
}

void GraphicsRenderer::FinalRender()
{
	m_quad1.Render();
	m_quad2.Render();
    m_deviceResources->Present();
}


// Screen Quad ------------------------------------------------------






