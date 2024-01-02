#include "Graphics.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


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

	void Graphics::DrawTestTriangle()
	{
		struct Vertex
		{
			float x;
			float y;
		};

		const Vertex vertices[] =
		{
			{ 0.0f, 0.5f },
			{ 0.5f, -0.5f },
			{ -0.5f, -0.5f }
		};

		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof( vertices );
		bd.StructureByteStride = sizeof( Vertex );

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices;

		HRESULT hr;

		//Bind Vertex Buffer
		GFX_THROW_FAILED( pDevice->CreateBuffer( &bd, &sd, &pVertexBuffer ) );
		const UINT stride = sizeof( Vertex );
		const UINT offset = 0u;
		pContext->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

		//Set Vertex Shader
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		GFX_THROW_FAILED( D3DReadFileToBlob( L"VertexShader.cso", &pBlob ) );
		GFX_THROW_FAILED( pDevice->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader ) );
		pContext->VSSetShader( pVertexShader.Get(), nullptr, 0u );

		//Bind Vertex Input Layout
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "Position", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		};
		GFX_THROW_FAILED( pDevice->CreateInputLayout( ied, (UINT)std::size( ied ), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout ) );
		pContext->IASetInputLayout( pInputLayout.Get() );
		

		//Set Pixel Shader
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		GFX_THROW_FAILED( D3DReadFileToBlob( L"PixelShader.cso", &pBlob ) );
		GFX_THROW_FAILED( pDevice->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader ) );
		pContext->PSSetShader( pPixelShader.Get(), nullptr, 0u );

		//Bind Render Target
		pContext->OMSetRenderTargets( 1u, pTarget.GetAddressOf(), nullptr);

		//Set Primitive Topology
		pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		//Configure Viewport
		D3D11_VIEWPORT vp;
		vp.Width = 1280;
		vp.Height = 720;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pContext->RSSetViewports( 1u, &vp );

		

		pContext->Draw( (UINT)std::size(vertices), 0u);
	}

}
