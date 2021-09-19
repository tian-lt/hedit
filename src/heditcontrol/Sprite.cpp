#include "pch.h"
#include "Sprite.h"
#include "DirectXHelper.h"

namespace DX = HeditControls::DX;
using namespace HeditControls;

const std::string ScreenQuad_VS = "\
struct VSInput{ \
	float4 pos: POSITION; \
	float2 uv: TEXCOORD; \
}; \
struct PSInput{ \
	float4 pos : SV_POSITION; \
	float2 uv: TEXCOORD; \
}; \
PSInput main(VSInput input){ \
	PSInput output; \
	output.pos = input.pos; \
	output.uv = input.uv; \
	return output; \
} \
";

const std::string ScreenQuad_PS = "\
Texture2D tex; \
SamplerState Sampler \
{ \
    Filter = MIN_MAG_MIP_LINEAR; \
    AddressU = Wrap; \
    AddressV = Wrap; \
}; \
struct PSInput{ \
	float4 pos : SV_POSITION; \
	float2 uv: TEXCOORD; \
}; \
float4 main(PSInput input) : SV_TARGET { \
	return tex.Sample(Sampler, input.uv); \
}\
";

Sprite::Sprite(DX::DeviceResources* deviceResources)
	: m_contentResView(nullptr)
	, m_vertData({
        Vertex({DirectX::XMFLOAT4(-1.f, -1.f, 0.5f, 1.f), DirectX::XMFLOAT2(0.f, 1.f)}), // left-bottom
        Vertex({DirectX::XMFLOAT4(-1.f,  1.f, 0.5f, 1.f), DirectX::XMFLOAT2(0.f, 0.f)}), // left-top
		Vertex({DirectX::XMFLOAT4( 1.f, -1.f, 0.5f, 1.f), DirectX::XMFLOAT2(1.f, 1.f)}), // right-bottom 
		Vertex({DirectX::XMFLOAT4( 1.f,  1.f, 0.5f, 1.f), DirectX::XMFLOAT2(1.f, 0.f)}), // right-top
        })
	, m_deviceResources(deviceResources)
{
	CreateDeviceIndependentResources();
	CreateDeviceDependentResources();
}

Sprite::~Sprite()
{
	DestroyDeviceDependentResources();
	DestroyDeviceIndpendentResources();
}

void Sprite::CreateDeviceIndependentResources()
{
	if (m_vsShaderBlob != nullptr)
		return;
	DX::CompileShader(ScreenQuad_VS, "main", "vs_5_0", &m_vsShaderBlob);
	DX::CompileShader(ScreenQuad_PS, "main", "ps_5_0", &m_psShaderBlob);
}

void Sprite::CreateDeviceDependentResources()
{
	if (m_vertBuffer != nullptr)
		return;
	auto d3ddev = m_deviceResources->GetD3DDevice();
	D3D11_BUFFER_DESC bufDesc;
	bufDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufDesc.ByteWidth = sizeof(m_vertData);
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = m_vertData.data();
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;
	DX::ThrowIfFailed(d3ddev->CreateBuffer(&bufDesc, &initData, &m_vertBuffer));

	static const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	DX::ThrowIfFailed(d3ddev->CreateInputLayout(layout, 2, m_vsShaderBlob->GetBufferPointer(), m_vsShaderBlob->GetBufferSize(), &m_inputLayout));
	DX::ThrowIfFailed(d3ddev->CreateVertexShader(m_vsShaderBlob->GetBufferPointer(), m_vsShaderBlob->GetBufferSize(), nullptr, &m_vsShader));
	DX::ThrowIfFailed(d3ddev->CreatePixelShader(m_psShaderBlob->GetBufferPointer(), m_psShaderBlob->GetBufferSize(), nullptr, &m_psShader));
}

void Sprite::DestroyDeviceIndpendentResources()
{
	m_vsShaderBlob.Reset();
	m_psShaderBlob.Reset();
}

void Sprite::DestroyDeviceDependentResources()
{
	m_vertBuffer.Reset();
	m_vsShader.Reset();
	m_psShader.Reset();
}

void Sprite::Render()
{
	if (m_contentResView != nullptr)
	{
		auto ctx3d = m_deviceResources->GetD3DDeviceContext();
		ctx3d->IASetInputLayout(m_inputLayout.Get());

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ctx3d->IASetVertexBuffers(0, 1, m_vertBuffer.GetAddressOf(), &stride, &offset);
		ctx3d->VSSetShader(m_vsShader.Get(), nullptr, 0);
		ctx3d->PSSetShader(m_psShader.Get(), nullptr, 0);
		ctx3d->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		ctx3d->PSSetShaderResources(0, 1, &m_contentResView);
		ctx3d->Draw(4, 0);
	}
}
