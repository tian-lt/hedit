#include "pch.h"
#include "DeviceResources.h"
#include "DirectXHelper.h"
#include "GraphicsRenderer.h"

namespace DX = HeditControls::DX;

using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace HeditControls;

GraphicsRenderer::GraphicsRenderer(DX::DeviceResources* deviceResources)
    : m_deviceResources(deviceResources)
{}

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
			Render();
			wait(1000); // TODO: keep staying at 60 FPS
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void GraphicsRenderer::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

void GraphicsRenderer::Render()
{
    auto ctx = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	ctx->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	ctx->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	ctx->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	ctx->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_deviceResources->Present();
}





