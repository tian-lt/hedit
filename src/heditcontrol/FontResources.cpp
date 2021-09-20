#include "pch.h"
#include "FontResources.h"
#include "DirectXHelper.h"

namespace DX = HeditControls::DX;
using namespace HeditControls;

FontResources::FontResources(DX::DeviceResources* deviceResources)
    : m_deviceResources(deviceResources)
{
    DX::ThrowIfFailed(m_deviceResources->GetDWriteFactory()->GetSystemFontSet(&m_sysFontSet));
    Microsoft::WRL::ComPtr<IDWriteFontSet> fontSet;
    DX::ThrowIfFailed(m_sysFontSet->GetMatchingFonts(L"Consolas", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STRETCH_NORMAL, DWRITE_FONT_STYLE_NORMAL, &fontSet));
    Microsoft::WRL::ComPtr<IDWriteFontFaceReference> fontFaceRef;
    DX::ThrowIfFailed(fontSet->GetFontFaceReference(0, &fontFaceRef));
    DX::ThrowIfFailed(fontFaceRef->CreateFontFace(&m_defaultFontFace));
}


