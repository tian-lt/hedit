#include "pch.h"
#include "DeviceResources.h"
#include "DirectXHelper.h"
#include "DocPage.h"

namespace DX = HeditControls::DX;
using namespace Microsoft::WRL;
using namespace HeditControls;

DocumentPage::DocumentPage(DX::DeviceResources* deviceResources)
    : m_deviceResources(deviceResources)
    , m_imgSize({1, 1})
{
	ComPtr<IDWriteTextFormat> textFormat;
	DX::ThrowIfFailed(m_deviceResources
        ->GetDWriteFactory()
        ->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			32.0f,
			L"en-US",
			&textFormat));
	DX::ThrowIfFailed(textFormat.As(&m_txtFormat));
	DX::ThrowIfFailed(m_txtFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));
    DX::ThrowIfFailed(m_txtFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING));
	DX::ThrowIfFailed(m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock));
    CreateWindowSizeDependentResources();
    CreateDeviceDependentResources();
}

void DocumentPage::CreateDeviceDependentResources()
{
    DX::ThrowIfFailed(m_rtOffScreen->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_brush));
}

void DocumentPage::DestroyDeviceDependentResources()
{
    m_brush.Reset();
}

void DocumentPage::ResetDeviceDependentResources()
{
    DestroyDeviceDependentResources();
    CreateDeviceDependentResources();
}

void DocumentPage::CreateWindowSizeDependentResources(uint32_t width, uint32_t height)
{
    if (width > 0 && height > 0)
    {
        m_imgSize.width = width;
        m_imgSize.height = height;
    }

    auto ctx3d = m_deviceResources->GetD3DDevice();
    D3D11_TEXTURE2D_DESC texDesc = { 0 };
    texDesc.ArraySize = 1;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.Width = m_imgSize.width;
    texDesc.Height = m_imgSize.height;
    texDesc.MipLevels = 1;
    texDesc.MiscFlags = 0;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    DX::ThrowIfFailed(ctx3d->CreateTexture2D(&texDesc, nullptr, &m_texOffScreen));

    D3D11_SHADER_RESOURCE_VIEW_DESC resDesc = {};
    resDesc.Format = texDesc.Format;
    resDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resDesc.Texture2D.MostDetailedMip = 0;
    resDesc.Texture2D.MipLevels = 1;
    DX::ThrowIfFailed(ctx3d->CreateShaderResourceView(m_texOffScreen.Get(), &resDesc, &m_texOffScreenView));

    IDXGISurface *dxgiSurface = nullptr;
    DX::ThrowIfFailed(m_texOffScreen->QueryInterface(&dxgiSurface));

    float dpi = m_deviceResources->GetDpi();
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(
            DXGI_FORMAT_UNKNOWN,
            D2D1_ALPHA_MODE_PREMULTIPLIED),
        dpi,
        dpi);

    DX::ThrowIfFailed(m_deviceResources->GetD2DFactory()->CreateDxgiSurfaceRenderTarget(
        dxgiSurface,
        &props,
        &m_rtOffScreen));
}

void DocumentPage::DestroyWindowSizeDependentResources()
{
    m_rtOffScreen.Reset();
    m_texOffScreen.Reset();
    m_texOffScreenView.Reset();
}

void DocumentPage::ResetWindowSizeDependentResources(uint32_t width, uint32_t height)
{
    DestroyWindowSizeDependentResources();
    CreateWindowSizeDependentResources(width, height);
}

void DocumentPage::Update()
{
    std::wstring text = L"Test Document Page, Test Document Page, Test Document Page, Test Document Page, Test Document Page";
	ComPtr<IDWriteTextLayout> textLayout;
    DX::ThrowIfFailed(m_deviceResources
        ->GetDWriteFactory()
        ->CreateTextLayout(
            text.c_str(),
            static_cast<UINT32>(text.length()),
            m_txtFormat.Get(),
            static_cast<float>(m_imgSize.width),
            static_cast<float>(m_imgSize.height),
			&textLayout
			)
		);

	DX::ThrowIfFailed(textLayout.As(&m_txtLayout));
}

void DocumentPage::Render()
{
    auto ctx2d = m_rtOffScreen;
    ctx2d->SaveDrawingState(m_stateBlock.Get());
    ctx2d->BeginDraw();
    ctx2d->Clear(D2D1::ColorF(D2D1::ColorF::Gray));
    ctx2d->SetTransform(D2D1::Matrix3x2F::Translation(0.f, 0.f));
    ctx2d->DrawTextLayout(D2D1::Point2F(0.f, 0.f), m_txtLayout.Get(), m_brush.Get());
    HRESULT hr = ctx2d ->EndDraw();
    if (hr != D2DERR_RECREATE_TARGET)
    {
        DX::ThrowIfFailed(hr);
    }
    ctx2d->RestoreDrawingState(m_stateBlock.Get());
}




