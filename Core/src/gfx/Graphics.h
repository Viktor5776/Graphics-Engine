#pragma once
#include "GraphicsException.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <random>
#include <memory>

namespace Hydro::gfx
{
	class Graphics
	{
		friend class Bindable;
	public:
		Graphics( HWND hWnd );
		Graphics( const Graphics& ) = delete;
		Graphics& operator=( const Graphics& ) = delete;
		~Graphics();
		void BeginFrame();
		void EndFrame();
		void ClearBuffer( float red, float green, float blue ) noexcept;
		void DrawIndexed( UINT count ) noexcept(!_DEBUG);
		void SetProjection( DirectX::FXMMATRIX proj ) noexcept;
		DirectX::XMMATRIX GetProjection() const noexcept;
	private:
		DirectX::XMMATRIX projection;
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
	};
}