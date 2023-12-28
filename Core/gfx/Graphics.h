#pragma once
#include "GraphicsException.h"
#include <d3d11.h>


namespace Hydro::gfx
{
	class Graphics
	{
	public:
		Graphics( HWND hWnd );
		Graphics( const Graphics& ) = delete;
		Graphics& operator=( const Graphics& ) = delete;
		~Graphics();
		void EndFrame();
		void ClearBuffer( float red, float green, float blue ) noexcept;
	private:
		ID3D11Device* pDevice = nullptr;
		IDXGISwapChain* pSwap = nullptr;
		ID3D11DeviceContext* pContext = nullptr;
		ID3D11RenderTargetView* pTarget = nullptr;
	};
}