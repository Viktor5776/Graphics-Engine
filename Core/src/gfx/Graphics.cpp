#include "Graphics.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "../../third/ImGui/imgui_impl_dx11.h"
#include "../../third/ImGui/imgui_impl_win32.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


namespace Hydro::gfx
{

	Graphics::Graphics( HWND hWnd )
		:
		projection( DirectX::XMMatrixIdentity() ),
		camera( DirectX::XMMatrixIdentity() )
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

		// create depth stensil state
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
		GFX_THROW_FAILED( pDevice->CreateDepthStencilState( &dsDesc, &pDSState ) );

		// bind depth state
		pContext->OMSetDepthStencilState( pDSState.Get(), 1u );

		// create depth stensil texture
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = 1280u;
		descDepth.Height = 720u;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		GFX_THROW_FAILED( pDevice->CreateTexture2D( &descDepth, nullptr, &pDepthStencil ) );

		// create view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;
		GFX_THROW_FAILED( pDevice->CreateDepthStencilView(
			pDepthStencil.Get(), &descDSV, &pDSV
		) );

		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = 1280.0f;
		vp.Height = 720.0f;
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
		pContext->OMSetRenderTargets( 1u, pTarget.GetAddressOf(), pDSV.Get() );
		const float color[] = { red, green, blue, 1.0f };
		pContext->ClearRenderTargetView( pTarget.Get(), color );
		pContext->ClearDepthStencilView( pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u );
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

}
