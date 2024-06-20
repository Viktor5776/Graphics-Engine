#include "Graphics.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <array>
#include "../../third/ImGui/imgui_impl_dx11.h"
#include "../../third/ImGui/imgui_impl_win32.h"
#include "Bindable/DepthStencil.h"
#include "Bindable/RenderTarget.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


namespace Hydro::gfx
{

	Graphics::Graphics( HWND hWnd, int width, int height )
		:
		width( width ),
		height( height ),
		projection( DirectX::XMMatrixIdentity() ),
		camera( DirectX::XMMatrixIdentity() )
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
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

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
		GFX_THROW_FAILED( pSwap->GetBuffer( 0, __uuidof(ID3D11Texture2D), &pBackBuffer ) );
		pTarget = std::shared_ptr<Bind::RenderTarget>{ new Bind::OutputOnlyRenderTarget( *this,pBackBuffer.Get() ) };

		// viewport always fullscreen (for now)
		D3D11_VIEWPORT vp;
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		pContext->RSSetViewports( 1u, &vp );

		// init imgui d3d impl
		ImGui_ImplDX11_Init( pDevice.Get(), pContext.Get() );
	}

	Graphics::~Graphics()
	{
		ImGui_ImplDX11_Shutdown();
	}

	void Graphics::BeginFrame( float red, float green, float blue ) noexcept
	{
		if( imguiEnabled )
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}
	}

	void Graphics::EndFrame()
	{
		HRESULT hr;
		if( imguiEnabled )
		{
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );

			if( ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}
		}
		

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

	void Graphics::DrawIndexed( UINT count ) noexcept(!_DEBUG)
	{
		pContext->DrawIndexed( count, 0u, 0u );
	}

	void Graphics::SetProjection( DirectX::FXMMATRIX proj ) noexcept
	{
		projection = proj;
	}

	DirectX::XMMATRIX Graphics::GetProjection() const noexcept
	{
		return projection;
	}

	void Graphics::SetCamera( DirectX::FXMMATRIX cam ) noexcept
	{
		camera = cam;
	}

	DirectX::XMMATRIX Graphics::GetCamera() const noexcept
	{
		return camera;
	}

	void Graphics::EnableImgui() noexcept
	{
		imguiEnabled = true;
	}

	void Graphics::DisableImgui() noexcept
	{
		imguiEnabled = false;
	}

	bool Graphics::IsImguiEnabled() const noexcept
	{
		return imguiEnabled;
	}

	UINT Graphics::GetWidth() const noexcept
	{
		return width;
	}

	UINT Graphics::GetHeight() const noexcept
	{
		return height;
	}

	std::shared_ptr<Bind::RenderTarget> Graphics::GetTarget()
	{
		return pTarget;
	}

}
