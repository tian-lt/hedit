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
	DX::ThrowIfFailed(m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock));
    CreateDeviceDependentResources();
}

void DocumentPage::CreateDeviceDependentResources()
{
    auto context = m_deviceResources->GetD2DDeviceContext();
    DX::ThrowIfFailed(context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_brush));
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

    auto context = m_deviceResources->GetD2DDeviceContext();
    D2D1_PIXEL_FORMAT pxFmt = {};
    pxFmt.alphaMode = D2D1_ALPHA_MODE_IGNORE;
    pxFmt.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    D2D1_BITMAP_PROPERTIES bmpProp = {};
    bmpProp.dpiX = m_deviceResources->GetDpi();
    bmpProp.dpiY = m_deviceResources->GetDpi();
    bmpProp.pixelFormat = pxFmt;
    DX::ThrowIfFailed(context->CreateBitmap(m_imgSize, bmpProp, &m_img));
}

void DocumentPage::DestroyWindowSizeDependentResources()
{
    m_img.Reset();
}

void DocumentPage::ResetWindowSizeDependentResources(uint32_t width, uint32_t height)
{
    DestroyWindowSizeDependentResources();
    CreateWindowSizeDependentResources(width, height);
}

void DocumentPage::Update()
{
    std::wstring text = L"Test Document Page";
	ComPtr<IDWriteTextLayout> textLayout;
	DX::ThrowIfFailed(m_deviceResources
        ->GetDWriteFactory()
        ->CreateTextLayout(
			text.c_str(),
			static_cast<UINT32>(text.length()),
			m_txtFormat.Get(),
			500.0f, // Max width of the input text.
			50.0f, // Max height of the input text.
			&textLayout
			)
		);

	DX::ThrowIfFailed(textLayout.As(&m_txtLayout));
}

void DocumentPage::Render()
{
    if (m_img != nullptr)
    {
        ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
        context->SaveDrawingState(m_stateBlock.Get());
        context->BeginDraw();

        DX::ThrowIfFailed(m_txtFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING));
        context->DrawTextLayout(D2D1::Point2F(0.f, 0.f), m_txtLayout.Get(), m_brush.Get());

        HRESULT hr = context->EndDraw();
        if (hr != D2DERR_RECREATE_TARGET)
        {
            DX::ThrowIfFailed(hr);
        }

        context->RestoreDrawingState(m_stateBlock.Get());
    }
}




