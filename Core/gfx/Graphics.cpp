#include "Graphics.h"
#pragma comment(lib, "d3d11.lib")

namespace Hydro::gfx
{

	Graphics::Graphics( HWND hWnd )
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		sd.OutputWindow = hWnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = 0;

		HRESULT hr;

		UINT swapCreateFlags = 0u;

#ifdef _DEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		GFX_THROW_FAILED( D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapCreateFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&pSwap,
			&pDevice,
			nullptr,
			&pContext
		) );

		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
		GFX_THROW_FAILED( pSwap->GetBuffer( 0, __uuidof( ID3D11Resource ), &pBackBuffer ) );
		GFX_THROW_FAILED( pDevice->CreateRenderTargetView( pBackBuffer.Get(), nullptr, &pTarget));
	}

	void Graphics::EndFrame()
	{
		HRESULT hr;
		if( FAILED( hr = pSwap->Present( 1u, 0u ) ) )
		{
			if( hr == DXGI_ERROR_DEVICE_REMOVED )
			{
				throw GFX_DEVICE_REMOVED_EXCEPT( pDevice->GetDeviceRemovedReason() );
			}
			else
			{
				GFX_THROW_FAILED( hr );
			}
		}
	}

	void Graphics::ClearBuffer( float red, float green, float blue ) noexcept
	{
		const float color[] = { red, green, blue, 1.0f };
		pContext->ClearRenderTargetView( pTarget.Get(), color);
	}

}
